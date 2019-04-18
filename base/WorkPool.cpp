#include "base/WorkPool.h"
#include "base/Thread.h"
#include <functional>
#include <stdio.h>

namespace base {
	
class WorkerThread
{
public:
	WorkerThread(WorkPool *pool, std::string name = "")
		: pool_(pool), name_(name),
		  thread_(std::bind(&WorkerThread::ThreadMain, this))
	{
		Thread::ThreadOptions options;
		options.stacksize = 0;
		options.joinable = false;
	//	thread_ = new base::Thread(std::bind(WorkerThread::ThreadMain,this), "WorkThread", options);
		thread_.Start();
	}

	void ThreadMain();
//private:
	base::Thread thread_;
	base::WorkPool *pool_;
	std::string name_;
};

void WorkerThread::ThreadMain()
{
	//const std::string name = sprintf(
  //    "%s-%d", name_.c_str(), base::Thread::CurrentId());
	//thread_->SetName(name.c_str());
	while(1)
	{
		Closure task = pool_->WaitForTask();
		if(task == NULL)
		{
			break;
		}
		task();
	}
	delete this;
}


WorkPool* WorkPool::instance = NULL;
base::Mutex WorkPool::instanceMutex_;

WorkPool::WorkPool(int coreThreads, int maxThreads)
	: threadNum_(coreThreads), coreThreads_(coreThreads), maxThreads_(maxThreads),cond_(mutex_)
{
	AddThreads(coreThreads_);
}

	
bool WorkPool::AddThreads(int num)
{
	for(int i = 0; i< num; i++)
	{
	//	printf("create thread--\n");
		//threadNum_++;
	//	int a = threadNum_;
	//	printf("threadNum_ number is %d\n", a);
		WorkerThread *thread = new WorkerThread(this, "WorkPool");
		//thread->Start();
		//threads_.push_back(thread);
	}
	return true;
}

bool WorkPool::PostTask(Closure task)
{
	{
	//	base::AutoLock lock(mutex_);

		/*int taskNum = taskQueue_.size();
		if (taskNum > 100)
		{
	//		return false;
		}*/
		
		taskQueue_.Push(std::move(task));

	//	int a = taskQueue_.Size();
		//printf("queue size is %d\n", a);
		
	//	taskQueue_.push(std::move(task));
	//	int a = threadNum_;

	//	printf("thread_num is %d\n", a);

		if (threadNum_ < maxThreads_ && taskQueue_.Size() > threadNum_ * 2)
		{
			threadNum_++;
			AddThreads(1);
		//	cond_.Signal();
			return true;
		}
//		printf("1111111111111--\n");
	}
	
	
	
	return true;
}

static int64_t GetMonotonicNow()
{
	LARGE_INTEGER counts;
	QueryPerformanceCounter(&counts);
	LARGE_INTEGER countsPerSecond;
	QueryPerformanceFrequency(&countsPerSecond);
	double result = static_cast<double>(counts.QuadPart);
	result *= (1.0) / countsPerSecond.QuadPart;
	return static_cast<int64_t>(result);
}


Closure WorkPool::WaitForTask()
{
	bool timeout = false;
	while (1)
	{

	/*	if (taskQueue_.Size() == 0)
		{
		
		int64_t end = GetMonotonicNow();


		int a = (int)end;

		printf("end is  %d miao\n", a);
	}*/
		bool timeoutEnable = threadNum_ > coreThreads_; 

		if(timeout && timeoutEnable)
		{
			threadNum_--;

		/*	int a = threadNum_;

			int64_t end = GetMonotonicNow();


			int ca = (int)end;
			printf("end is  %d miao\n", ca);
			printf("thread num is %d\n", a);*/
			return NULL;
		}
		Closure task;
		bool result = (timeoutEnable ? taskQueue_.Poll(task, 1 * 1000 * 1000) : taskQueue_.Poll(task));
		if (result)
		{
			return std::move(task);
		}
		timeout = true;
	}
	return NULL;
}




bool WorkPool::RemoveThreads()
{
	return true;
}



	
	
	
	
}