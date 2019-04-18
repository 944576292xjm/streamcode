#include "base/Listener.h"
#include "base/TcpConnection.h"
#include "base/EventLoopManager.h"
namespace base {

Listener::Listener(EventMessageLoop* loop, EndPoint& address, EventLoopManagerPtr loopManager)
	: loop_(loop), address_(address), socket_(address), loopManager_(loopManager)
{
	//socket_->Set();
	//loop_->WatchEvent(socket_.GetFd());
	socket_.SetNonBlocking();
	socket_.SetReuseAddr();
}

bool Listener::Listen()
{
	//SocketFactory factory;
//	socket_ = factory.CreateListenSocket(address_)
	if (socket_.Fd() == -1)
	{
		int c = 0;
	}
	//TcpConnectionPtr connection = std::make_shared<TcpConnection>(loopManager_->GetOneLoop(), 1);
	loop_->AddEvent(socket_.Fd(), EventRead, std::bind(&Listener::Accept, this));
//	loop_->PostTask(std::bind(&Listener::CloseConnection,this, connection));
	bool ret = socket_.Listen();
	if (ret == false)
	{
		int c = 0;
	}
	
	return true;
}

void Listener::Accept()
{
	int fd = socket_.Accept();

	//auto tcpConnection = new TcpConnection(loop_,fd);
	TcpConnectionPtr connection = std::make_shared<TcpConnection>(loopManager_->GetOneLoop(),fd);

	if (connectedCallback_)
	{
		connectedCallback_(connection);
	}
	connection->SetCloseCallback(std::bind(&Listener::CloseConnection, this, std::placeholders::_1));
	connectionManager_.insert(connection);
	connection->Start();
	//	tcpConnection->setReadCallback();

		//tcpConnection->set
}

//	SetCloseCallback(NewConnectionCallback callback)
void Listener::CloseConnection(TcpConnectionPtr conn)
{
	auto iter = connectionManager_.find(conn);
	if (iter != connectionManager_.end())
	{
		connectionManager_.erase(*iter);
	}

}



}