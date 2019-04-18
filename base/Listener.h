#ifndef _BASE_LISTENER_H
#define _BASE_LISTENER_H

#include "base/EventMessageLoop.h"
#include "base/Socket.h"
#include "base/Callback.h"
#include "base/EndPoint.h"
#include <memory>
#include <set>

namespace base {
	
class Listener
{
public:
	//typedef std::function<void(int)> ConnectedCallback;

	Listener(EventMessageLoop* loop, EndPoint& address, EventLoopManagerPtr loopManager);
	
	~Listener()
	{

		int c = 0;
	}

	void SetConnectedCallback(NewConnectionCallback callback) {connectedCallback_ = std::move(callback);}
	
	bool Listen();

private:
	void Accept();
//	SetCloseCallback(NewConnectionCallback callback)
	void CloseConnection(TcpConnectionPtr conn);

	EventMessageLoop* loop_;

	ListenSocket socket_;
	
	EndPoint address_;
	
	NewConnectionCallback connectedCallback_;

	EventLoopManagerPtr loopManager_;

	std::set<TcpConnectionPtr> connectionManager_;

};
	
	
	
}

#endif