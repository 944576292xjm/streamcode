#ifndef _BASE_LOCK_H_
#define _BASE_LOCK_H_

#ifdef __Win32__
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif __linux__
#include <pthread.h>
#endif

namespace base {
	
class Mutex
{
public:
	Mutex();
	~Mutex();
	
	void Lock();
	void UnLock();
	bool Try();
	
#ifdef __Win32__
	CRITICAL_SECTION fMutex;
#elif __linux__
	pthread_mutex_t fMutex;
#endif
};

class AutoLock {
public:
	explicit AutoLock(Mutex& lock) : lock_(lock) 
	{
		lock_.Lock();	
	}
	
	~AutoLock()
	{
		lock_.UnLock();
	}
	
private:
    Mutex &lock_;
};

}

#endif