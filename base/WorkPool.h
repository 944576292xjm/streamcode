#ifndef __BASE_WORK_POOL_H
#define __BASE_WORK_POOL_H

#include <atomic>
#include <queue>
#include "base/ThreadPool.h"
#include "base/WorkPool.h"
#include "base/Mutex.h"
#include "base/Cond.h"
#include "base/BlockingQueue.h"

namespace base {
	
class WorkPool : public ThreadPool	
{
public:
	typedef BlockingQueue<Closure> TaskQueue;

	static WorkPool* SingleInstance()
	{
		if (instance == NULL)
	    {
			base::AutoLock lock(instanceMutex_);
	    	if (instance == NULL)
	    	{
	       		instance = new WorkPool(1, 2);
	    	}
	    }
		return instance;
	}
	

//	WorkPool(int coreThreads, int maxThreads);

	Closure WaitForTask();

//	Closure PollQueue(int msecond);
	
	virtual bool AddThreads(int num);
	
	virtual bool RemoveThreads();
	
	virtual bool PostTask(Closure task);

	virtual bool PostDelayTask(Closure task, int64_t mseconds) { return true; }

	virtual bool PostIdleTask(Closure task) { return true; }

	virtual bool PostIdleDelayTask(Closure task, int64_t mseconds) { return true; }
		
private:
	WorkPool(int coreThreads, int maxThreads);
	WorkPool(const WorkPool&);
	WorkPool& operator=(const WorkPool&) {}

	static WorkPool* instance;
	static base::Mutex instanceMutex_;

private:

	base::Mutex mutex_;
	base::Cond cond_;

	int coreThreads_;
	int maxThreads_;

	std::atomic<int> threadNum_; 
	TaskQueue taskQueue_;
};

		
}


#endif