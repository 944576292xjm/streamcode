#ifndef BASE_MESSAGE_LOOP_H_
#define BASE_MESSAGE_LOOP_H_

#include <memory>
#include <queue>
#include "base/Callback.h"
#include "base/BlockingQueue.h"
#include "base/TimeTick.h"
#include "base/Thread.h"

namespace base
{



class MessageLoop
{
public:

	class PendingTask
	{
	public:
		PendingTask(Closure task, int64_t delay, bool idle) : task_(std::move(task)), delayRunTime_(delay), idle_(idle) {}
		~PendingTask() {}

		bool operator<(const PendingTask &otherTask) const
		{
			return delayRunTime_ > otherTask.delayRunTime_;
		}

		bool IsDelay() { return delayRunTime_.IsValid(); }

		bool IsIdle() { return idle_; }

		TimeTick DelayTime() const  { return delayRunTime_; }

		void Run()
		{
			task_();
		}

	private:

		Closure task_;
		bool idle_;
		bool isDelay_;
		TimeTick delayRunTime_;
	};
	
	typedef std::function<void()> Closure;
	typedef std::queue<PendingTask> WorkQueue;
	typedef base::BlockingQueue<PendingTask> TaskQueue;
	typedef std::priority_queue<PendingTask> DelayTaskQueue;

	enum Type
	{
		TYPE_DEFAULT,
		//	TYPE_UI,
		TYPE_IO
	};

//	explicit MessageLoop();
//	virtual ~MessageLoop();

	//static EventLoop*  Create(MessageLoopType type);
	MessageLoop() : threadId_(Thread::CurrentId()) {}

	bool PostSyncTask(Closure task);


	bool PostTask(Closure task);

	bool PostDelayTask(Closure task, int64_t useconds);

	bool PostIdleTask(Closure task);

	bool PostIdleDelayTask(Closure task, int64_t useconds);

	bool BelongsToCurrentThread();

	virtual void ScheduleWork() = 0;
	virtual void RunLoop() = 0;


	//void RunLoop();

private:
//	bool AddToIncomingQueue(PendingTask& task);
//	bool AddToWorkQueue(PendingTask& task);
	bool AddToDelayQueue(PendingTask& task);
	bool AddToIdleQueue(PendingTask& task);
	void InjectWorkQueue();


private:
//	base::Mutex taskQueueLock_;
	TaskQueue incomingQueue_;
	WorkQueue workQueue_;
	DelayTaskQueue delayTaskQueue_;
	WorkQueue idleTaskQueue_;
	int threadId_;
protected:
	bool DoTask();
	bool DoDelayTask(TimeTick &nextTaskTime);
	bool DoIdleTask();
	
private:


	bool RunOneTask(PendingTask& pendingTask);

};

}

#endif
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
