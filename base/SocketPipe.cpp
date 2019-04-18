#include "base/SocketPipe.h"
#include "base/Socket.h"
namespace base {

SocketPipe::SocketPipe()
	//: _server(SocketFactory::CreateListenSocket(EndPoint("127.0.0.1", 0))),
//	  _client(SocketFactory::CreateClientSocket(EndPoint("127.0.0.1", 0)))
{
	EndPoint initEndPoint(0, "127.0.0.1");
	EndPoint localEndPoint;

	ListenSocket listenSocket(initEndPoint);
	listenSocket.SetReuseAddr();
	//listenSocket.Init();
	int ret = 0;
	listenSocket.GetLocalAddress(&localEndPoint);
	ret = listenSocket.Listen();
	if (ret == false)
	{
		printf("listen error--\n");
	}

	//writeSocket_.Init();
//	writeSocket_.Bind(initEndPoint);

	ret = writeSocket_.Connect(localEndPoint);
	if (ret == false)
	{
		printf("connect error--\n");
	}
	readSocket_.Attach(listenSocket.Accept());

	//writeSocket_.SetSocketSendBufSize(50*1024*1024);
	//writeSocket_.SetSocketRcvBufSize(50 * 1024 * 1024);
	//readSocket_.SetSocketRcvBufSize(50 * 1024 * 1024);
	//readSocket_.SetSocketSendBufSize(50 * 1024 * 1024);
	writeSocket_.SetNoDelay();
	writeSocket_.SetNonBlocking();
//	readSocket_.SetReuseAddr();
	readSocket_.SetNonBlocking();
}

int SocketPipe::Write(const void *buf, unsigned int size)
{
	unsigned int len = 0;
	int err =  writeSocket_.Send(buf, size, &len);
	if (err != 0)
	{
		int b = 0;
	}
	if (len == 0)
	{
		int a = 0;
	}
	return 0;
}

int SocketPipe::Read(void *buf, unsigned int size)
{
	unsigned int len = 0;
	int err =  readSocket_.Read(buf, size, &len);
	if (err != 0)
	{
		int c = 0;
	}
	return 0;
}



}