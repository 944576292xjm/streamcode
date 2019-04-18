#ifndef __BASE_EVENT_LOOP_MANAGER_H
#define __BASE_EVENT_LOOP_MANAGER_H

#include <vector>
#include <atomic>
#include <memory>
#include "base/EventMessageLoop.h"
#include "base/EventLoopThread.h"

namespace base {
	
class EventLoopManager
{
public:
	EventLoopManager(int n) : threadNum_(n)
	{
	//new EventLoopThread();
	}
	
	bool Start()
	{
		for(int i = 0; i < threadNum_; i++)
		{
			std::unique_ptr<EventLoopThread> thread = std::make_unique<EventLoopThread>();
			thread->Start();
			threadQueue_.push_back(std::move(thread));
		}
		return true;
	}
	
	EventMessageLoop* GetOneLoop()
	{
		if (threadCounter_ >= threadQueue_.size())
		{
			threadCounter_ = 0;
		}
		return threadQueue_[threadCounter_++]->GetMessageLoop();
	}

	EventMessageLoop* GetLoopByOrder(unsigned int n)
	{
		if (n >= threadQueue_.size())
		{
			return NULL;
		}
		return threadQueue_[n]->GetMessageLoop();
	}

	int GetLoopNum()
	{
		return threadNum_;
	}
		
private:
	std::vector<std::shared_ptr<EventLoopThread>> threadQueue_;
	std::atomic<unsigned int> threadCounter_;
	int threadNum_;
};
	

}

#endif