#include "base/TcpServer.h"

#include <thread>
#include "base/Thread.h"

namespace base {
	

	
TcpServer::TcpServer(EndPoint& address, int ioThreadsNum, int acceptThreadsNum) 
	: address_(address), ioThreadsNum_(ioThreadsNum), acceptThreadsNum_(acceptThreadsNum), 
  	  acceptManager_(new EventLoopManager(acceptThreadsNum)), 
	  ioManager_(new EventLoopManager(ioThreadsNum))
{
	assert(acceptManager_);
	assert(ioManager_);
}

bool TcpServer::Start()	
{
	
	//acceptGroup_ = std::make_shared<EventThreadPool>(acceptThreadsNum_);
	
/*	if(ioThreadsNum_ <= 0)
	{
		acceptThreadsNum_ = std::thread::hardware_concurrency();
	}*/
	//ioGroup_ = std::make_shared<EventThreadPool>(acceptThreadsNum_);
	
//	acceptGroup_.Start();
//	ioGroup_.Start()
	;
	acceptManager_->Start();
	ioManager_->Start();

	for (int i = 0; i < acceptManager_->GetLoopNum(); i++)
	{
		std::unique_ptr<Listener> listener = std::make_unique<Listener>(acceptManager_->GetLoopByOrder(i), address_, ioManager_);
		if (newConneciontCallback_)
		{
			listener->SetConnectedCallback(std::bind(&TcpServer::NewConnection, this, std::placeholders::_1));
	//		listener->SetConnectedCallback(newConneciontCallback_);
		}
		listener->Listen();
		listenManager_.push_back(std::move(listener));
	}

	
	/*for(int i = 0; i < acceptThreadsNum_; i++)
	{

		EventLoopThread *thread = new EventLoopThread();
		thread->Start();
		Listener *listener = new Listener(thread->GetMessageLoop(), address_);
		std::unique<Listener> listen = std::make_unique<Listener>();
	//	EventMessageLoop* loop = acceptGroup_->PickMessageLoop(i);
	//	listener = std::make_shared<Listener>(loop);
	//	listener_.SetConnectCallback(std::bind(NewConnection, this, _1));
		listener->SetConnectedCallback(std::bind(&TcpServer::NewConnection, this, std::placeholders::_1));
		listener->Listen();
		listenManager_
	}*/

	/*for (int i = 0; i < ioThreadsNum_; i++)
	{
		//EventLoopThread *thread = new EventLoopThread();
		std::shared_ptr<EventLoopThread> thread(new EventLoopThread());
		thread->Start();
		threadQueue_.push_back(std::move(thread));
	//	thread->Start();
	}*/

	//base::Thread *thread_ = new Thread(std::bind(&EventMessageLoop::RunLoop, &loop_));
	//thread_->Start();

	//listener_ = std::make_unique<Listener>(&loop_, address_);
	//listener_->SetConnectedCallback(std::bind(&TcpServer::NewConnection, this, std::placeholders::_1));
	//listener_->Listen();

	return true;
}

/*void TcpServer::NewConnection(int fd)
{
	EventMessageLoop *loop = NULL;
	//std::shared_ptr<TcpConnection> connection = std::make_shared<TcpConnection>(&loop_, fd);
	if (threadCounter_ >= threadQueue_.size())
	{
		threadCounter_ = 0;
	}
	loop = threadQueue_[threadCounter_++]->GetMessageLoop();

	//TcpConnection *connect = new TcpConnection(loop, fd);
	std::shared_ptr<TcpConnection> connection = std::make_shared<TcpConnection>(loop, fd);


	//con->SetReadCallback();
	//con->SetWriteCallback();
}*/

void TcpServer::NewConnection(TcpConnectionPtr conn)
{
	conn->SetReadCallback(newReadCallback_);
	newConneciontCallback_(conn);
}
	
	
	
	
		
}