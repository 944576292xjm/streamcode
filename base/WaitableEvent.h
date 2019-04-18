#ifndef __BASE_WAITABLE_EVENT_H_
#define __BASE_WAITABLE_EVENT_H_

#include <stdint.h>
#include "base/Mutex.h"
#include "base/Cond.h"

namespace base {

class WaitableEvent
{
public:
	WaitableEvent() : mutex_(), cond_(mutex_), isSigned(false), waitingNum_(0) {}
	
	void Signal();
	void Broadcast();
	void Wait();
	void WaitTime(int64_t useconds);

private:

	bool isSigned;
	int waitingNum_;
	base::Mutex mutex_;
	base::Cond cond_;
	
};

}

#endif