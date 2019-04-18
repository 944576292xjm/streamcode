#include "base/Socket.h"

namespace base {

	


bool Socket::Create(int af, int type, int protocol)
{
	sockfd_ = ::socket(af, type, protocol);
	if (sockfd_ == INVALID_SOCKET)
	{
		sockfd_ = -1;
	}
//	int n = GetLastError();
	return (sockfd_ != INVALID_SOCKET);
}
	
bool Socket::Bind(EndPoint& endPoint)
{
	struct sockaddr address;
	// socklen_t address_len;
	if (!endPoint.ToSockAddr(&address))
	{
		return false;
	}
	return ::bind(sockfd_, &address, sizeof(struct sockaddr)) == 0;
}

/*bool Socket::Connect(SocketAddress address)
{
	return ::connect(sockfd_, static_cast<struct sockaddr *>(&address.addr_), sizeof(struct sockaddr));
}*/

void Socket::SetNoDelay()
{
    int one = 1;
	int err = ::setsockopt(sockfd_, SOL_SOCKET, TCP_NODELAY, (char*)&one, sizeof(int));
//	assert(err == 0);
}
void Socket::SetReuseAddr()
{
	int one = 1;
    int err = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, (char*)&one, sizeof(int));
	//assert(err = 0);
}
void Socket::SetKeepAlive()
{
	int one = 1;
    int err = ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE, (char*)&one, sizeof(int));
	//assert(err == 0);   
}

void Socket::SetSocketSendBufSize(unsigned int size)
{
	int sendSize = size;
    int err = ::setsockopt(sockfd_, SOL_SOCKET, SO_SNDBUF, (char*)&sendSize, sizeof(int));
	//assert(error == 0);
}

void Socket::SetSocketRcvBufSize(unsigned int size)
{
	int recvSize = size;
    int err = ::setsockopt(sockfd_, SOL_SOCKET, SO_RCVBUF, (char*)&recvSize, sizeof(int));
	//assert(err == 0);
}

void Socket::SetNonBlocking()
{
#ifdef __Win32__
	unsigned long one = 1;
	int err = ::ioctlsocket(sockfd_, FIONBIO, &one);
#elif __linux__	
	int flag = ::fcntl(fFileDesc, F_GETFL, 0);
    int err = ::fcntl(fFileDesc, F_SETFL, flag | O_NONBLOCK);
#endif
	//assert(err == 0);
}


int Socket::GetErrno()
{
#ifdef __Win32__
	return ::GetLastError();
#elif __linux__
	return errno;
#endif
}


/*int Socket::GetErrno()
{
#ifdef __Win32__
	int winErr = ::GetLastError();
	switch (winErr)
	{
		case ERROR_FILE_NOT_FOUND: return ENOENT;
		case ERROR_PATH_NOT_FOUND: return ENOENT;
		case WSAEINTR:      return EINTR;
		case WSAENETRESET:  return EPIPE;
		case WSAENOTCONN:   return ENOTCONN;
		case WSAEWOULDBLOCK:return EAGAIN;
		case WSAECONNRESET: return EPIPE;
		case WSAEADDRINUSE: return EADDRINUSE;
		case WSAEMFILE:     return EMFILE;
		case WSAEINPROGRESS:return EINPROGRESS;
		case WSAEADDRNOTAVAIL: return EADDRNOTAVAIL;
		case WSAECONNABORTED: return EPIPE;
		case 0:             return 0;
		default:            return ENOTCONN;
	}
#elif __linux__
	return errno;
#endif
}*/

SocketError Socket::TranslateSocketError(int error)
{
#ifdef __Win32__
	switch (error)
	{
		case 0: return kSocketOk;
		case WSAEWOULDBLOCK: 
			return kSocketPending;
		case WSAECONNRESET:
		case WSAENETRESET :
		case WSAECONNABORTED :
		case ERROR_BROKEN_PIPE :
			return kSocketConnectRest;
	}
	return kSocketFailed;
#elif __linux__
	switch (error)
	{
	case 0:
		return kSocketOk;
	case EAGAIN:
//#if EWOULDBLOCK != EAGAIN
	case EWOULDBLOCK:
		return kSbSocketPending;
	case ECONNRESET:
	case ENETRESET:
	case EPIPE:
		return kSocketConnectRest;
	}
	return kSocketFailed;
#endif
}

ListenSocket::ListenSocket(EndPoint& endPoint) : endPoint_(endPoint)
{
	Socket::Create(endPoint_.IsIpv4() ? AF_INET : 0, SOCK_STREAM, 0);
	int ret = Bind(endPoint_);
	if (ret == false)
	{
		int c = 0;
	}

}

bool ListenSocket::Listen()
{ 
	if (sockfd_ == kSocketInvalid)
	{
		return false;
	}
	
//	SetNonBlocking();
//	SetReuseAddr();
	return listen(sockfd_, 10) == 0; 
}

int ListenSocket::Accept(EndPoint* endpoint)
{
	if (sockfd_ == kSocketInvalid)
	{
		return kSocketInvalid;
	}
	struct sockaddr addr;
	socklen_t address_length = sizeof(addr);
	int fd = ::accept(sockfd_, &addr, &address_length);
	if(fd <= 0)
	{
	//	int winErr = ::GetLastError();
	//	return kSocketInvalid;
		return TranslateSocketError(GetErrno());
	}

	if(endpoint)
	{
		endpoint->FromSockAddr(&addr);
	}
	return fd;
}

bool ClientSocket::Connect(EndPoint endPoint)
{
	if (!Socket::Create(AF_INET, SOCK_STREAM, 0))
	{
		return false;
	}
	if (!localEndPoint_.IpAddress().empty() && localEndPoint_.Port() >= 0)
	{
		if (Bind(localEndPoint_) == false)
		{
			return false;
		}
	}

	struct sockaddr addr;
	endPoint.ToSockAddr(&addr);
	if (int ret = ::connect(sockfd_, &addr, sizeof(addr)) != 0)
	{
	//	int winErr = ::GetLastError();
		//::closesocket(sockfd_);
		return false;
	}
	return true;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}