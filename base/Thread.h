#ifndef  _BASE_THREAD_H_
#define  _BASE_THREAD_H_

#include <string>
#include<functional>

#ifdef __Win32__
#include <process.h>   
#include <Windows.h>
#elif __linux__
#include <sys/prctl.h>
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#endif

namespace base 
{

#ifdef __Win32__
typedef DWORD ThreadId;
#elif
typedef pid_t ThreadId;
#endif

class Thread
{
public:
    typedef std::function<void()> ThreadFunc;

	class ThreadOptions
	{
	public:
		ThreadOptions() : stacksize(0), joinable(true) {}//, priority(0)
		int64_t  stacksize;
		//int  priority;
		bool joinable;
	};

	//Thread(ThreadFunc func, std::string name, ThreadOptions option);

	Thread(ThreadFunc func)
		: func_(std::move(func)), fJoined(false) {}

	Thread(ThreadFunc func, std::string name) 
		: func_(std::move(func)), name_(name), fJoined(false) {}

	Thread(ThreadFunc func, ThreadOptions& options)
		: func_(std::move(func)), options_(options), fJoined(false) {}

	Thread(ThreadFunc func, std::string name , ThreadOptions& options)
		: func_(std::move(func)), name_(name), options_(options), fJoined(false) {}
   
	~Thread() {}
	
	
	bool Start();
	
	void Join();

	std::string GetThreadName() { return name_; }
	
	static int CurrentId();
	
	static void YieldCurrentThread();
	
	static void Sleep(int64_t inMsec);
	
	static void SetThreadName(const char* name);

private:
	std::string name_;
	ThreadFunc func_;
	
#ifdef __Win32__
    HANDLE fThreadId;
#elif __linux
	pthread_t fThreadId;
#endif

    bool fJoined;	
	ThreadOptions options_;

#ifdef __Win32__
	static unsigned int WINAPI ThreadStart(void *params);
#elif __linux__
	static void* ThreadStart(void *params);
#endif
};

#ifdef __Win32__
#pragma pack(push, 8)
typedef struct tagTHREADNAME_INF {
	DWORD dwType;      // Must be 0x1000.
	LPCSTR szName;     // Pointer to name (in user addr space).
	DWORD dwThreadID;  // Thread ID (-1=caller thread).
	DWORD dwFlags;     // Reserved for future use, must be zero.
} THREADNAME_INFO;
#endif
	
	
}











#endif