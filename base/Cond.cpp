#include "base/Cond.h"

namespace base {
	
Cond::Cond(base::Mutex& mutex) : mutex_(mutex)
{
#ifdef __linux__
	pthread_cond_init(&fcond,NULL);
#elif __Win32__
	InitializeConditionVariable(&fcond);
#endif
}

Cond::~Cond()
{
#ifdef __linux__
	pthread_cond_destroy(&fcond);
#elif __Win32__
	//
#endif
}

#ifdef __Win32__
bool Cond::Signal()
{
	WakeConditionVariable(&fcond);
	return true;
}

bool Cond::Broadcast()
{
	WakeAllConditionVariable(&fcond);
	return true;
}

int Cond::Wait()
{
	return WaitTime(-1);
}

int Cond::WaitTime(int64_t useconds)
{
	DWORD time;
	if(useconds > 0)
	{
		time = static_cast<DWORD>(useconds /1000);
	}
	else
	{
		time = INFINITE;
	}
	
	bool result = SleepConditionVariableCS(&fcond, &mutex_.fMutex, time);
	if(result)
	{
		return 1;
		//return kConditionSignaled;
	}
	
	if(GetLastError() == ERROR_TIMEOUT)
	{
		//return kConditionTimeOut;
		return 0;
	}
		 
	//return kConditionFailed;
	return -1;
}
#elif __linux__
bool Cond::Signal()
{
	return pthread_cond_signal(&fcond) == 0;
}

bool Cond::Broadcast()
{
	return pthread_cond_broadcast(&fcond) == 0;
}

int Cond::Wait()
{
	int result = pthread_cond_wait(&fcond, &mutex->fMutex);
	if(result == 0)
	{
		return 0;
	}
	return -1;
}

int Cond::WaitTime(int useconds)	
{
	struct timespec timeout;
	timeout.tv_sec = mseconds/1000000;
	timeout.tv_nsec = (mseconds%1000000) * 1000;
	
	int result = pthread_cond_timedwait(&fcond, &mutex->fMutex, &timeout);
	if(result == 0)
	{
		return 0;
	}
	
	/*if(GetLastError() == ERROR_TIMEOUT)
	{
		return kConditionTimeOut;
	}*/
		 
	//return kConditionFailed;
	return -1;
}
#endif
	
	
	
	
	
	
	
	
	
}