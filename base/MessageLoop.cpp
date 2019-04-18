#include "base/MessageLoop.h"



namespace base {

	/*EventLoop*  MessageLoop::Create(MessageLoopType type)
	{
		if(type == TYPE_IO)
		{
			return new EventMessageLoop();
		}

	}*/

bool MessageLoop::PostTask(Closure task)
{
	//std::shared_ptr pendingTask = std::make_shared<PendingTask>(task);
	PendingTask pendingTask(task, 0, false);
	incomingQueue_.Push(pendingTask);
	ScheduleWork();
	return true;
}

bool MessageLoop::PostSyncTask(Closure task)
{
	if (BelongsToCurrentThread())
	{
		task();
	}
	else
	{
		PostTask(std::move(task));
	}
	return true;
}

bool MessageLoop::PostDelayTask(Closure task, int64_t useconds)
{
//	int c = (int)(useconds / 1000000);
//	printf("useconds is %d\n", c);
	int64_t a = useconds + TimeTick::GetMonotonicNow();
//	int b = (int)(a / 1000000);
//	printf("time is %d\n", b);
	PendingTask pendingTask(task, a, false);
	incomingQueue_.Push(pendingTask);
	ScheduleWork();
	return true;
}

bool MessageLoop::PostIdleTask(Closure task)
{
	PendingTask pendingTask(task, 0, true);
	incomingQueue_.Push(pendingTask);
	ScheduleWork();
	return true;
}

bool MessageLoop::PostIdleDelayTask(Closure task, int64_t useconds)
{
	PendingTask pendingTask(task, useconds + TimeTick::GetMonotonicNow(), true);
	incomingQueue_.Push(pendingTask);   
	ScheduleWork();
	//ScheduleWork();
	return true;
}

bool MessageLoop::BelongsToCurrentThread()
{
	if (threadId_ != Thread::CurrentId())
	{
		return false;
	}
	return true;
}

bool MessageLoop::AddToDelayQueue(PendingTask& task)
{
	delayTaskQueue_.push(task);
	//	taskQueue_.Push(pendingTask);
	//	ScheduleWork();
	return true;
}

bool MessageLoop::AddToIdleQueue(PendingTask& task)
{
	idleTaskQueue_.push(task);
	return true;
}

bool MessageLoop::RunOneTask(PendingTask& pendingTask)
{
	if (pendingTask.IsIdle())
	{
		AddToIdleQueue(pendingTask);
		return false;
	}

	pendingTask.Run();
	return true;
}

void MessageLoop::InjectWorkQueue()
{
	if(!workQueue_.empty())
	{
		return;
	}

	int a = incomingQueue_.Size();
	int b = workQueue_.size();

	//printf("a is %d, b is %d\n", a, b);

	incomingQueue_.Swap(workQueue_);
}


bool MessageLoop::DoTask()
{
	while (1)
	{
	
		InjectWorkQueue();
		if (workQueue_.empty())
		{
			return false;
		}

		do
		{
			PendingTask pendingTask = workQueue_.front();
			workQueue_.pop();
			if (pendingTask.IsDelay())
			{
			//	printf("add to delayqueue---\n");
				AddToDelayQueue(pendingTask);
				//delayTaskQueue_.push(task);
			}
			else
			{
				if (RunOneTask(pendingTask))
				{
				//	printf("real --\n");
					return true;
				}
			}
		} while (!workQueue_.empty());
	}
	return false;
}

bool MessageLoop::DoDelayTask(TimeTick &nextTaskTime)
{
	if (delayTaskQueue_.empty())
	{
		return false;
	}
	//printf("DoDelayTask--\n");
	TimeTick runTime = delayTaskQueue_.top().DelayTime();


		int b = (int)(runTime() / 1000000);
	//	printf("time is %d\n", b);

	if (runTime > TimeTick::Now())
	{
		nextTaskTime = runTime;
		return false;
	}

	TimeTick now = TimeTick::Now();

	//int c = (int)(now() / 1000000);
	//printf("now is %d\n", c);

//	int b = (int)(runTime() / 1000000);
//	printf("time is %d\n", b);

	

	PendingTask task = delayTaskQueue_.top();
	delayTaskQueue_.pop();

//	ss++;

//	printf("ss is %d\n",ss);

	if (!delayTaskQueue_.empty())
	{
		nextTaskTime = delayTaskQueue_.top().DelayTime();
	}
	return RunOneTask(task);
}

bool MessageLoop::DoIdleTask()
{
	if (idleTaskQueue_.empty())
	{
		return false;
	}

	PendingTask task = idleTaskQueue_.front();
	task.Run();
	return true;
}


}


	
	
	
	
	
