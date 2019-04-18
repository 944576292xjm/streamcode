#ifndef _BASE_COND_H
#define _BASE_COND_H

#include "Mutex.h"
#include <stdint.h>

#ifdef __Win32__
#include <windows.h>
#elif __linux__
#include <pthread.h>
#include <errno.h>
#endif

namespace base {
	
class Cond
{
public:
	explicit Cond(base::Mutex& mutex);
	~Cond();
	
	bool Signal();
	bool Broadcast();
	int Wait();
	int WaitTime(int64_t useconds);
	
private:
	base::Mutex& mutex_;

#ifdef __Win32__
	CONDITION_VARIABLE fcond;
#elif __linux__
	pthread_cond_t fcond;
#endif
	
	
	
};
	
}

#endif