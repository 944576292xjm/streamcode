#ifndef __BASE_CLOSURE_H
#define __BASE_CLOSURE_H

#include <functional>
#include <memory>
//#include "base/TcpConnection.h"
//#include "base/EventLoopManager.h"
//#include "base/EventMessageLoop.h"
namespace base {
	class TcpConnection;
	class EventLoopManager;

	typedef std::function<void()> Closure;
	typedef std::function<void(int)> EventCallback;
	typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef std::function<void(TcpConnectionPtr)> NewConnectionCallback;
	typedef std::shared_ptr<EventLoopManager>  EventLoopManagerPtr;

}

#endif