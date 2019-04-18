#include "base/Thread.h"


namespace base
{

bool Thread::Start()
{
#ifdef __Win32__
	fThreadId = (HANDLE)_beginthreadex(NULL, static_cast<unsigned int>(options_.stacksize), ThreadStart, (void *)this, 0, NULL);
	if (fThreadId <= 0)
	{ 
		return false;
	}
	return true;
#elif __linux
	pthread_attr_t attributes;
	int result = pthread_attr_init(&attributes);
	if (result != 0)
	{
		return false;
	}

	pthread_attr_setdetachstate(&attributes,
		(options_.joinable ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED));

	if (options_.stacksize > 0)
	{
		pthread_attr_setstacksize(&attributes, stack_size);
	}

	result = pthread_create(&fThreadId, &attributes, threadStart, (void *)this);

	pthread_attr_destroy(&attributes);
	if (result != 0)
	{
		return false;
	}
	return true;
#endif
}
#ifdef __Win32__
unsigned int Thread::ThreadStart(void *params)
#elif __linux__
void* Thread::ThreadStart(void *params)
#endif
{
	Thread *thread = static_cast<Thread *>(params);

	auto name = thread->GetThreadName();
	if (!name.empty())
	{
		SetThreadName(name.c_str());
	}

	if (thread->func_ == NULL)
	{
		int id = base::Thread::CurrentId();
		//int a = 0;
	}

	thread->func_();
#ifdef __Win32__
	return 0;
#endif
}

void Thread::SetThreadName(const char *threadName)
{
#ifdef __Win32__
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = threadName;
	info.dwThreadID = -1;
	info.dwFlags = 0;

	__try
	{
		RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (DWORD*)&info);
	}
	__except (EXCEPTION_CONTINUE_EXECUTION)
	{
	}
#elif __linux__
	prctl(PR_SET_NAME, name);
#endif
}

int Thread::CurrentId()
{
#ifdef __Win32__
	return static_cast<int>(GetCurrentThreadId());
#elif __linux__	
	return static_cast<int>(syscall(SYS_gettid));
#endif
}

void Thread::Join()
{
	//assert(!fJoined);
	fJoined = true;
#ifdef __Win32__							
	DWORD theErr = ::WaitForSingleObject(fThreadId, INFINITE);
//	assert(theErr == WAIT_OBJECT_0);
#elif __linux__
	pthread_join((pthread_t)fThreadID, NULL);
#endif
}

void Thread::YieldCurrentThread()
{
#ifdef __Win32__
	::Sleep(0);
#elif __linux
	sched_yield();
#endif
}

void Thread::Sleep(int64_t usecond)
{
#ifdef __Win32__
	::Sleep(static_cast<DWORD>(usecond/1000));
#elif __linux__
	usleep(usecond);
#endif
}

}
	
	
	
	
	
	
