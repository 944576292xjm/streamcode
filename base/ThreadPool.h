#ifndef __BASE_THREAD_POOL_H
#define __BASE_THREAD_POOL_H

#include "base/Callback.h"

namespace base {
	


class ThreadPool
{
public:
	//ThreadPool(int coreThreads, int maxThreads);
	virtual bool AddThreads(int num) = 0;
	
	virtual bool RemoveThreads() = 0;
	
	virtual bool PostTask(Closure task) = 0;

	virtual bool PostDelayTask(Closure task, int64_t mseconds) = 0;

	virtual bool PostIdleTask(Closure task) = 0;

	virtual bool PostIdleDelayTask(Closure task, int64_t mseconds) = 0;
};	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}














#endif