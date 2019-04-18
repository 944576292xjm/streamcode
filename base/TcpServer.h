#ifndef _BASE_TCP_SERVER_H_
#define _BASE_TCP_SERVER_H_

#include "base/EndPoint.h"
#include "base/Listener.h"
#include "base/TcpConnection.h"
#include "base/Callback.h"
#include <memory>
#include <functional>
#include <base/EventLoopManager.h>
#include "base/EventLoopThread.h"

namespace base {
	
class TcpServer
{
public:
	TcpServer(EndPoint &address, int ioThreadsNum = 0, int acceptThreadsNum = 1);
	
	bool Start();

	void SetNewConnectionCallback(NewConnectionCallback callback)
	{
		newConneciontCallback_ = callback;
	}

	void SetReadCallback(NewConnectionCallback callback)
	{
		newReadCallback_ = callback;
	}

	
private:	

	void NewConnection(TcpConnectionPtr conn);
	//typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	//typedef std::map<int, TcpConnectionPtr> ConnectionMap;
	//void NewConnection(int fd);
	NewConnectionCallback newConneciontCallback_;
	NewConnectionCallback newReadCallback_;
	//std::unique_ptr<Listener> listener_;
	//Test1:
//	EventMessageLoop loop_;

//	std::vector<std::shared_ptr<EventLoopThread>> threadQueue_;

	

	//unsigned int threadCounter_;
	//
	EndPoint address_;
	int ioThreadsNum_;
	int acceptThreadsNum_;

	EventLoopManagerPtr acceptManager_;
	EventLoopManagerPtr ioManager_;
	//EventLoopManager acceptManager_;
//	EventLoopManager ioManager_;
	std::vector<std::unique_ptr<Listener>> listenManager_;

};
	
}


#endif