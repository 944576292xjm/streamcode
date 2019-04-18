#include "base/EventMessageLoop.h"

#include "base/SelectPoller.h"
#include "base/SocketFactory.h"

namespace base {

EventMessageLoop::EventMessageLoop() : pipeWrap_()
{
#ifdef __linux__ 

#elif __Win32__
	poller_ = std::make_unique<SelectPoller>();
#endif

	AddEvent(pipeWrap_.readFd(), EventRead, std::bind(&EventMessageLoop::HandleWakeUpRead, this));
}	
	
int EventMessageLoop::AddEvent(int sockfd, int event, EventCallback callback)
{
	if (!BelongsToCurrentThread())
	{
	//	PostTask([this, sockfd, event, callback]() {
	//		AddEvent(sockfd, event, callback);
	//	});
		PostTask(std::bind(&EventMessageLoop::AddEvent,this, sockfd, event, callback));
		return 1;
	}
	return poller_->AddEvent(sockfd, event, callback);
//	ScheduleWork();
}

int EventMessageLoop::ModifyEvent(int sockfd, int event)
{
	if (!BelongsToCurrentThread())
	{
		PostTask(std::bind(&EventMessageLoop::ModifyEvent, this, sockfd, event));
		return 1;
	}
	return poller_->ModifyEvent(sockfd, event);
}

int EventMessageLoop::RemoveEvent(int sockfd)
{
	if (!BelongsToCurrentThread())
	{
		PostTask(std::bind(&EventMessageLoop::RemoveEvent, this, sockfd));
		return 1;
	}
	return poller_->RemoveEvent(sockfd);
}

void EventMessageLoop::RunLoop()
{
	while(1)
	{
		TimeTick delayWorkTime;
		bool doWorkSucess = DoTask();
		
		Poller::Result result = poller_->PollTime(0);
		if(result == Poller::ProcessIO)
		{
			doWorkSucess |= true;
		}
		
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
			if(delay > 0)
			{
				result = poller_->PollTime(delay);
			}
		}
		else
		{
			poller_->Poll();
		}	
	}
	printf("yichule---\n");
}

void EventMessageLoop::ScheduleWork()
{
	char buf = 1;
	int err = pipeWrap_.Write(&buf, 1);
	int winErr = ::GetLastError();
	if (err != 0)
	{
		printf("wakeup write error--\n");
	}
}

void EventMessageLoop::HandleWakeUpRead()
{
//	return;
	char buf;
	int err = pipeWrap_.Read(&buf, 1);
	int winErr = ::GetLastError();
	if (err != 0)
	{
		printf("wakeup read error--\n");
	}
//	printf("heihei--\n");
}
	
	
	
	
	
	
	
}
	