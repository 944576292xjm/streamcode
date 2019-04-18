#include "base/EventLoopThread.h"


namespace base {
	

	
EventLoopThread::EventLoopThread()
	: thread_(std::bind(&EventLoopThread::RunLoop, this)), messageLoop_(NULL)
{

}

EventLoopThread::~EventLoopThread()
{
	//messageLoop_->Quit();
	//thread_->Join();
}

bool EventLoopThread::Start()
{
	if (!thread_.Start())
	{
		return false;
	}

	event_.Wait();

	if (messageLoop_ == NULL)
	{
		return false;
	}
	return true;
}

EventMessageLoop* EventLoopThread::GetMessageLoop()
{
	return messageLoop_;
}




	
void EventLoopThread::RunLoop()
{
	
	//messageLoop_ = MessageLoop::Create(MessageLoop::TYPE_IO);
	EventMessageLoop loop;
//	threadId_ = Thread::CurrentId();
	//SetThreadName();
	messageLoop_ = &loop;
	event_.Signal();
	loop.RunLoop();
	
//	messageLoop_ = NULL;
}	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}