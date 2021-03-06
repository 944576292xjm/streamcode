#if 0

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
#include "rtmp/FlvReader.h"
using namespace base;

class EchoSession
{
public:
	~EchoSession()
	{
		int c = 0;
	}
public:
	int a = 0;

};
#if 1
class RtmpServer
{
public:
	RtmpServer(EndPoint point) : server_(point, 50)
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
		std::shared_ptr<RtspSession> session(new RtspSession(conn));

		conn->SetReadCallback(std::bind(&RtspSession::OnRead, session, std::placeholders::_1));
		conn->SetAttr("EchoSession", session);
		//conn->SetSendRate(1000 * 1);
		int b = 0;
	}

	void HandleRead(TcpConnectionPtr conn)
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
	}

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
	EndPoint endPoint(554);
	RtmpServer server(endPoint);



	//TcpServer server(endPoint, 5);
	server.Start();

	system("pause");
#endif

	Thread::Sleep(60 * 60 * 60 * 1000000);
	return 0;
}



#endif