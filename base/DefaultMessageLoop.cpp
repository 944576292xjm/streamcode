#include "base/DefaultMessageLoop.h"


namespace base {


void DefaultMessageLoop::ScheduleWork()
{
	event_.Signal();
}
	
void DefaultMessageLoop::RunLoop()
{
	while(1)
	{
		TimeTick delayWorkTime;
		bool doWorkSucess = DoTask();
		
		doWorkSucess |= DoDelayTask(delayWorkTime);
		
		if(doWorkSucess)
		{
			continue;
		}
		
		doWorkSucess |= DoIdleTask();
		
		if(doWorkSucess)
		{
			continue;
		}
	
		if(delayWorkTime.IsValid())
		{
			int64_t delay = delayWorkTime - TimeTick::Now();
			if(delay)
			{
				event_.WaitTime(delay);
			}
		}
		else
		{
			event_.Wait();
		}
		
	}
}
	
	

}