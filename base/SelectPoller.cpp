#include "base/SelectPoller.h"


namespace base {


	
SelectPoller::SelectPoller()
{
	FD_ZERO(&readSet);
	FD_ZERO(&writeSet);
	FD_ZERO(&excptionSet);
}
	
int SelectPoller::AddEvent(int fd, int type, EventCallback callback)
{
	if(type & EventRead)
	{
		FD_SET(fd, &readSet);
	}
	else if(type & EventWrite)
	{
		FD_SET(fd, &writeSet);
	}
	
	//eventMap_.emplace(fd, std::make_shared<EventBase>(fd,type,callback));
	eventMap_[fd] = std::make_shared<EventBase>(fd, type, callback);
	return 0;
}

int SelectPoller::ModifyEvent(int fd, int type)
{
	FD_CLR(fd, &readSet);
	FD_CLR(fd, &writeSet);
	FD_CLR(fd, &excptionSet);
	
	if(type & EventRead)
	{
		FD_SET(fd, &readSet);
	}
	if(type & EventWrite)
	{
		FD_SET(fd, &writeSet);
	}
	return 0;
}

int SelectPoller::RemoveEvent(int fd)
{
	FD_CLR(fd, &readSet);
	FD_CLR(fd, &writeSet);
	FD_CLR(fd, &excptionSet);
	eventMap_.erase(fd);
	return 0;
}

Poller::Result SelectPoller::Poll()
{
	return PollTime(-1);
}

Poller::Result SelectPoller::PollTime(int64_t useconds)
{
	fd_set readWorkSet, writeWorkSet, exceptWorkSet;
	struct timeval timeout; 
	struct timeval *timeoutptr = NULL;
	

	FD_ZERO(&readWorkSet);
	FD_ZERO(&writeWorkSet);
	FD_ZERO(&exceptWorkSet);
	
	readWorkSet   = readSet;
	writeWorkSet  = writeSet;
	exceptWorkSet = excptionSet;
	
	if(useconds >= 0)
	{
		timeout.tv_sec  = static_cast<long>(useconds /1000000);
		timeout.tv_usec = static_cast<long>(useconds %1000000);
		timeoutptr = &timeout;
	}
	else if(useconds == -1)
	{
		timeoutptr = NULL;
	}
	else
	{
		return Error;
	}

	int numberFds = ::select(0, &readWorkSet, &writeWorkSet, &exceptWorkSet, timeoutptr);
//	int numberFds = ::select(0, NULL, NULL, NULL, 0);
	if(numberFds < 0)
	{
		int winErr = ::GetLastError();
		return Error;
	}
	else if(numberFds == 0)
	{
		return TimedOut;
	}
	
	for(auto item : eventMap_) //lock
	{
		int event = EventDummy;
		int fd = item.first;
		
		if(FD_ISSET(fd, &readWorkSet))
		{
			event |= EventRead; 
		}
		if(FD_ISSET(fd, &writeSet))
		{
			event |= EventWrite; 
		}
		if(FD_ISSET(fd, &exceptWorkSet))
		{
			event |= EventError;
		}
		
		if(event != EventDummy)
		{
			item.second->attachEvent(event);
			callbackList_.push_back(item.second);
		//	callbackList_.push_back();
		//	item.second(event);
		}
	}

	for (auto iter : callbackList_)
	{
		int event = iter->attach;
		iter->callback_(event);
	}
	callbackList_.clear();

	
	return ProcessIO;
}


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
}