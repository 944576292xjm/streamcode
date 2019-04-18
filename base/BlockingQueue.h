#ifndef __BASE_BLOCKING_QUEUE_H
#define __BASE_BLOCKING_QUEUE_H

#include <queue>
#include "base/Mutex.h"
#include "base/Cond.h"

namespace base {

template <class T>
class BlockingQueue
{
public:
	BlockingQueue() : mutex_(), cond_(mutex_) {}

	void Push(T& t)
	{
		base::AutoLock lock(mutex_);
		queue_.push(t);
		cond_.Signal();
	}

	void Push(T&& t)
	{
		base::AutoLock lock(mutex_);
		queue_.push(std::move(t));
		cond_.Signal();
	}

	bool Poll(T& item, int useconds = -1)
	{
		base::AutoLock lock(mutex_);
		if (queue_.empty())
		{
			useconds < 0 ? cond_.Wait() : cond_.WaitTime(useconds);
			if (queue_.empty())
			{
				return false;
			}
		}
		item = queue_.front();
		queue_.pop();
		return true;
	}

	int Size()
	{
		base::AutoLock lock(mutex_);
		return queue_.size();
	}

	void Swap(std::queue<T>& t)
	{
		base::AutoLock lock(mutex_);
		queue_.swap(t);
	}

private:
	Cond cond_;
	Mutex mutex_;
	std::queue<T> queue_;
};

}

#endif