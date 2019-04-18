#include "base/TimeTick.h"


#ifdef __Win32__
#include <windows.h>
#elif __linux__
#include <time.h>
#endif

namespace base {
	

#ifdef __linux__
int64_t TimeStamp::GetMonotonicNow()
{
	struct timespic time;
	if(clock_gettime(CLOCK_MONOTONIC, &time) != 0)
	{
		return 0;
	}
	return static_cast<int64_t>(time->tv_sec) * 1000000 +
			static_cast<int64_t>(time->tv_nsec)/1000;
}
#elif __Win32__
int64_t TimeTick::GetMonotonicNow()
{
	LARGE_INTEGER counts;
	QueryPerformanceCounter(&counts);
	LARGE_INTEGER countsPerSecond;
	QueryPerformanceFrequency(&countsPerSecond);
	double result = static_cast<double>(counts.QuadPart);
	result *= (1000.0 * 1000.0) / countsPerSecond.QuadPart;
	return static_cast<int64_t>(result);
}
#endif
	
TimeTick TimeTick::Now()
{
	return TimeTick(GetMonotonicNow());
}	
	
	
	

}