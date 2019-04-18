#ifndef __BASE_SOCKET_H
#define __BASE_SOCKET_H

#ifdef __Win32__
#include<winsock2.h>
#include<ws2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#elif __linux__	
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#endif
#include "base/EndPoint.h"
#include <errno.h>
#include <assert.h>

#ifdef __Win32__
struct iovec {
	u_long  iov_len; // this is not the POSIX definition, it is rather defined to be
	char FAR*   iov_base; // equivalent to a WSABUF for easy integration into Win32
};
#endif

namespace base
{

typedef enum SocketError
{
	kSocketInvalid = -1,
	kSocketOk = 0,
	kSocketPending,
	kSocketConnectRest,
	kSocketFailed,
		
}SocketError;

class Socket
{
	
public:
	explicit Socket() : sockfd_(kSocketInvalid) {}
	explicit Socket(int sockfd) : sockfd_(sockfd) {}
	virtual ~Socket()
	{
		if (sockfd_ != kSocketInvalid)
		{
			::closesocket(sockfd_);
			sockfd_ = kSocketInvalid;
		}
	}

	bool Create(int af, int type, int protocol);

	int Fd() { return sockfd_; }

	void Attach(int fd) { sockfd_ = fd; }

	bool Bind(EndPoint& address);
//	bool Connect(EndPoint& address);
	void SetNoDelay();
	void SetReuseAddr();
	void SetKeepAlive();
	void SetSocketSendBufSize(unsigned int size);
	void SetSocketRcvBufSize(unsigned int size);
	void SetNonBlocking();

	bool GetLocalAddress(EndPoint* endPoint)
	{
		struct sockaddr addr;
		socklen_t addrLen = sizeof(addr);
		if (::getsockname(sockfd_, &addr, &addrLen) != 0)
		{
			return false;
		}
		return endPoint->FromSockAddr(&addr);
	}

	SocketError TranslateSocketError(int error);

	static int GetErrno();

	bool EINTRERR()
	{
#ifdef __Win32__
		return GetErrno() == WSAEINTR;
#elif __linux__
		return GetErrno() == EINTR;
#endif
	}
	//int Send(const char *data, int length);

	int Send(const void *data, const unsigned int size, unsigned int *sendLength)
	{
		if (sockfd_ == kSocketInvalid)
		{
		//	return 
		}
		int err = 0;
		do {
			err = ::send(sockfd_, reinterpret_cast<const char *>(data), size, 0);
		} while (err == -1 && EINTRERR());

		if (err == -1)
		{
			return TranslateSocketError(GetErrno());
		}
		*sendLength = err;
		return 0;
	}

	int Read(void *data, const unsigned int size, unsigned int *recnLen)
	{
		assert(data != NULL);

		int err = 0;
		do {
			err = ::recv(sockfd_, (char *)data, size, 0);
		} while (err == -1 && EINTRERR());

		if (err == -1)
		{
			//return GetErrno();
			return TranslateSocketError(GetErrno());
		}
		else if (err == 0)
		{
			//Todo
			return kSocketConnectRest;
		}
		*recnLen = err;
		return kSocketOk;
	}


	/*int Readv(struct iovec* iov, const int numIovs, unsigned int* readLen)
	{

		unsigned int all, t = 0;
		unsigned int send;
		*readLen = 0;
		while (numIovs)
		{
			r = Read(iov->iov_base, iov->iov_len, &send);
			if (r == 0)
			{
				*readLen += send;
			}
				return r;
			t += r;
			iov++;
			count--;

		}
		return t;
	}*/

	int ReadV(struct iovec* iov, const int numIovs, unsigned int *readLen)
	{
		assert(iov != NULL);

		int err;
		do {
#ifdef __Win32__
			DWORD bytesRead;
			DWORD flags = 0;
			err = ::WSARecv(sockfd_, (LPWSABUF)iov, numIovs, &bytesRead, &flags, NULL, NULL);
			if (err == 0)
			{
				err = bytesRead;
			}
#elif
			err = ::readv(sockfd_, iov, numIovs);
#endif
		} while (err == -1 && EINTRERR());

		if (err == -1)
		{
			//return GetErrno();
			return TranslateSocketError(GetErrno());
		}
		else if (err == 0)
		{
			//Todo
			return kSocketConnectRest;
		}
		*readLen = err;

		return 0;
	}

#if 1
	int WriteV(const struct iovec* iov, const int numIovs, unsigned int* sendLen)
	{
		assert(iov != NULL);
		unsigned int allSendBytes = 0;
		int err = 0;
		for (int i = 0; i < numIovs; i++)
		{
			unsigned int sendBytes = 0;
			if (iov[i].iov_len == 0)
			{
				break;
			}

			err = Send(iov[i].iov_base, iov[i].iov_len, &sendBytes);
			if (err == kSocketOk) 
			{
				if (sendBytes == 0)
				{
					int t = 0;
				}
				allSendBytes += sendBytes;
			}
			else if (err == kSocketPending)
			{
				allSendBytes += sendBytes;
				*sendLen = allSendBytes;
				return err;
			}
			else
			{
				return err;
			}
		}
		*sendLen = allSendBytes;
		return err;
	}

#else
	int WriteV(const struct iovec* iov, const int numIovs, unsigned int* sendLen)
	{
		assert(iov != NULL);

		int err;
		do {
#ifdef __Win32__
			DWORD theBytesSent = 0;
			err = ::WSASend(sockfd_, (LPWSABUF)iov, numIovs, &theBytesSent, 0, NULL, NULL);

		//	printf("theBytesSent is %d\n", theBytesSent);
			if (err == 0)
			{
				err = theBytesSent;
			}
#elif
			err = ::writev(sockfd_, iov, numIovs);
#endif
		} while (err == -1 && EINTRERR());

		if (err == -1)
		{
			//return GetErrno();
			return TranslateSocketError(GetErrno());
		}
		*sendLen = err;

		return 0;
	}

#endif

protected:
	int sockfd_;
};

class TcpSocket : public Socket
{
public:
	//TcpSocket() {}
	TcpSocket(int sockfd) : Socket(sockfd) {}
};


class ListenSocket : public Socket
{
public:
	//ListenSocket() {}
	ListenSocket(EndPoint& endPoint);

	bool Listen(); //{ return listen(sockfd_, 10) == 0; }
	
	int Accept(EndPoint* endpoint = NULL);

private:
	EndPoint endPoint_;
};

class ClientSocket : public Socket
{
public:
	//ListenSocket() {}
	ClientSocket() {}
	//ClientSocket(int fd) : Socket(fd) {}
	ClientSocket(EndPoint endPoint) : localEndPoint_(endPoint){}

	bool Connect(EndPoint endPoint);
private:
	EndPoint localEndPoint_;
private:
};


}

#endif

