#ifndef _BASE_RATE_LIMTER_H
#define _BASE_RATE_LIMTER_H

#include "base/TimeTick.h"

namespace base {

#define RateLimiter_MIN(a,b) ((a)<(b))?(a):(b)
#define RateLimiter_MAX(a,b) ((a)>(b))?(a):(b)

class RateLimiter
{
public:
	RateLimiter() {}
	RateLimiter(double permitsPerSecond)
	{
		SetNewRate(permitsPerSecond);
		//nextFreeTicketMicros = Now();
	}

	bool resync(int64_t nowMicros)
	{
		if (nowMicros > nextFreeMicros)
		{
			storedPermits = RateLimiter_MIN(maxPermits,
				storedPermits + (nowMicros - nextFreeMicros) / stableIntervalMicros);
			nextFreeMicros = nowMicros;
			return true;
		}
		return false;
	}

	void SetNewRate(double permitsPerSecond)
	{
		//	double oldMaxPermits = maxPermits;
		//	maxPermits = permitsPerSecond;
		//	stableIntervalMicros = 1000000 / permitsPerSecond;
		int64_t now = TimeTick::GetMonotonicNow();
		if (maxPermits == 0)
		{
			storedPermits = 0;
			nextFreeMicros = now;
		}
		else
		{
			resync(now);
			/*if(storedPermits > 0)
			{
				storedPermits = RateLimiter_MIN(maxPermits,
					storedPermits + (nowMicros - nextFreeTicketMicros) / stableIntervalMicros);
		//		storedPermits + (nowMicros - nextFreeTicketMicros) / stableIntervalMicros);
			//	storedPermits = storedPermits * maxPermits / oldMaxPermits;
			}*/
			/*	else
				{
				//	storedPermits = storedPermits * oldMaxPermits / maxPermits;
				}*/
		}

		maxPermits = permitsPerSecond;
		stableIntervalMicros = 1000000 / permitsPerSecond;
	}

	void SetNewParm(double permitsPerSecond)
	{
		//	maxPermits = permitsPerSecond;
		//	stableIntervalMicros = 1000000 / permitsPerSecond;
	}

	int64_t Accquire(int requiredPermits)
	{
		int64_t now = TimeTick::GetMonotonicNow();
		if (!resync(now))
		{
		//	int64_t time = nextFreeMicros - now;
	//		Thread::Sleep(time);
		//	return nextFreeMicros - now;
		}

		int64_t returnValue = nextFreeMicros - now;

		//	storedPermits = RateLimiter_MIN(maxPermits,
			//	storedPermits + (nowMicros - nextFreeTicketMicros) / stableIntervalMicros);

		if (requiredPermits <= storedPermits)
		{
			//wait = 0;
			storedPermits -= requiredPermits;
			nextFreeMicros = now;
			return 0;
		}

		int64_t wait = (int64_t)((requiredPermits - storedPermits) * stableIntervalMicros);
		nextFreeMicros += wait;
		storedPermits = 0;

		return RateLimiter_MAX(returnValue, 0);
	}

private:
	double maxPermits = 0;
	double permitsPerSecond;
	double stableIntervalMicros;
	double storedPermits;
	int64_t nextFreeMicros;

};

}
#endif