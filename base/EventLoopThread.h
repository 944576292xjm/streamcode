#ifndef __BASE_MESSAGE_LOOP_THREAD_H
#define __BASE_MESSAGE_LOOP_THREAD_H


#include "base/EventMessageLoop.h"
#include "base/WaitableEvent.h"
#include "base/Thread.h"

namespace base {
	
class EventLoopThread
{
public:
	
	EventLoopThread();
	~EventLoopThread();

	
	bool Start();
	EventMessageLoop* GetMessageLoop();
		
private:
	void RunLoop();
	
	
	
	
private:
	EventMessageLoop* messageLoop_;
	
	base::Thread thread_;

	ThreadId threadId_;
	
	base::WaitableEvent event_;

//	base::Cond cond_;
	
};
	
	
		
}

#endif