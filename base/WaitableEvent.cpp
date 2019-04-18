#include "base/WaitableEvent.h"
#include <stdio.h>
namespace base {

void WaitableEvent::Wait()
{
	base::AutoLock lock(mutex_);
	if (isSigned)
	{
		printf("isSigned\n");
		isSigned = false;
		return;
	}
//	printf("waitingNum_ + 1\n");
	waitingNum_++;
	cond_.Wait();
//	printf("waitingNum_ - 1\n");
	waitingNum_--;
}

void WaitableEvent::Signal()
{
	base::AutoLock lock(mutex_);
	if (waitingNum_ == 0)
	{
		isSigned = true;
		return;
	}
	cond_.Signal();
}

void WaitableEvent::Broadcast()
{
	base::AutoLock lock(mutex_);
	if (waitingNum_ == 0)
	{
		isSigned = true;
		return;
	}
	cond_.Broadcast();
}

void WaitableEvent::WaitTime(int64_t useconds)
{
	base::AutoLock lock(mutex_);
	if (isSigned)
	{
		isSigned = false;
		return;
	}
	waitingNum_++;
	cond_.WaitTime(useconds);
	waitingNum_--;
}

}