#include "base/Mutex.h"

namespace base {
	
Mutex::Mutex()
{
#ifdef __Win32__
    ::InitializeCriticalSection(&fMutex);
#elif __linux__
	pthread_mutex_init(&fMutex, NULL);
#endif
}

Mutex::~Mutex()
{
#ifdef __Win32__
    ::DeleteCriticalSection(&fMutex);
#elif __linux__
    pthread_mutex_destroy(&fMutex);
#endif
}

void Mutex::Lock()
{
#ifdef __Win32__
    ::EnterCriticalSection(&fMutex);
#elif __linux__
	pthread_mutex_lock(&fMutex);
#endif
}

void Mutex::UnLock()
{
#ifdef __Win32__
    ::LeaveCriticalSection(&fMutex);
#elif __linux
    pthread_mutex_unlock(&fMutex);
#endif
}

	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}