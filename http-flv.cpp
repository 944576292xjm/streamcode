

#include "base/Socket.h"
#include  "base/Thread.h"
#include "base/Cond.h"
#include "base/WaitableEvent.h"
#include "base/WorkPool.h"
#include "base/SocketFactory.h"
#include "base/DefaultMessageLoop.h"
#include "base/EventMessageLoop.h"
#include <atomic>
#include <functional>
#include "base/EndPoint.h"
#include "base/TcpServer.h"
#include "base/Callback.h"
#include "base/RateLimiter.h"
#include "rtmp/RtmpSession.h"
#include "rtmp/RtspSession.h"
#include "rtmp/HttpFlvSession.h"
#include "rtmp/FlvReader.h"
using namespace base;



class RtmpServer
{
public:
	RtmpServer(EndPoint point) : server_(point, 5)
	{
		server_.SetNewConnectionCallback(std::bind(&RtmpServer::NewConnection, this, std::placeholders::_1));
		//	server_.SetReadCallback(std::bind(&EchoServer::HandleRead, this, std::placeholders::_1));
	}

	void NewConnection(TcpConnectionPtr conn)
	{
		/*std::shared_ptr<RtmpSession> session(new RtmpSession(conn));

		//	session->a = 2;
		conn->SetReadCallback(std::bind(&RtmpSession::OnRecv, session, std::placeholders::_1));
		conn->SetAttr("EchoSession", session);*/
		std::shared_ptr<RtmpSession> session(new RtmpSession(conn));

		conn->SetReadCallback(std::bind(&RtmpSession::OnRecv, session, std::placeholders::_1));
		conn->SetAttr("RtmpSession", session);
		//conn->SetSendRate(1000 * 1);
		int b = 0;
	}

	bool Start()
	{
		return server_.Start();
	}

private:
	TcpServer server_;
};

#if 1
class HttpServer
{
public:
	HttpServer(EndPoint point) : server_(point, 5)
	{
		server_.SetNewConnectionCallback(std::bind(&HttpServer::NewConnection, this, std::placeholders::_1));
	//	server_.SetReadCallback(std::bind(&EchoServer::HandleRead, this, std::placeholders::_1));
	}

	void NewConnection(TcpConnectionPtr conn)
	{
		/*std::shared_ptr<RtmpSession> session(new RtmpSession(conn));

	//	session->a = 2;
		conn->SetReadCallback(std::bind(&RtmpSession::OnRecv, session, std::placeholders::_1));
		conn->SetAttr("EchoSession", session);*/
		std::shared_ptr<HttpFlvSession> session(new HttpFlvSession(conn));

		conn->SetReadCallback(std::bind(&HttpFlvSession::OnRecv, session, std::placeholders::_1));
		conn->SetAttr("HttpFlvSession", session);
		//conn->SetSendRate(1000 * 1);
		int b = 0;
	}

/*	void HandleRead(TcpConnectionPtr conn)
	{
		std::shared_ptr<EchoSession> session = any_cast<std::shared_ptr<EchoSession>>(conn->GetAttr("EchoSession"));
		char *buffer = (char *)malloc(conn->inputBuffer_.Size());
		int len = conn->inputBuffer_.Size();
		conn->inputBuffer_.Read(buffer, conn->inputBuffer_.Size());
	//	conn->tt();
		conn->SendB(buffer, len);
		//	conn->inputBuffer_.AddReadPos(conn->inputBuffer_.Size());
		//	conn->Send(conn->inputBuffer_.GetDataPtr(), conn->inputBuffer_.Size());
		//conn->inputBuffer_.AddReadPos(conn->inputBuffer_.Size());
		int c = 0;
	}*/

	bool Start()
	{
		return server_.Start();
	}

private:
	TcpServer server_;
};
#endif
int main()
{
	/*int64_t wait;
	RateLimiter rateLimiter(1000 * 1000 * 1);

	wait = rateLimiter.Accquire(1000 * 1000 * 10);
	printf("%I64d\n", wait);
	wait = rateLimiter.Accquire(1000 * 1000 * 1);
	printf("%I64d\n", wait);
	wait = rateLimiter.Accquire(1);
	printf("%I64d\n", wait);
	wait = rateLimiter.Accquire(1);
	printf("%I64d\n", wait);
	wait = rateLimiter.Accquire(1);
	printf("%I64d\n", wait);*/


	//Thread::Sleep(600000 * 60 * 1000000);


	//FlvReader reader("E:\\xiqu.flv");
//	bool ret = reader.Open();
//	Thread::Sleep(60 * 60 * 60 * 1000000);
#if 1
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("socket start error --\n");
		return 0;
	}
	//EndPoint endPoint(1935);
	EndPoint endPoint(80);
	HttpServer server(endPoint);

	EndPoint rtmpendPoint(1935);
	RtmpServer rtmpserver(rtmpendPoint);



	//TcpServer server(endPoint, 5);
	server.Start();
	rtmpserver.Start();

	system("pause");
#endif

	Thread::Sleep(60 * 60 * 60 * 1000000);
	return 0;
}



