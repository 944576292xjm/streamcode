#ifndef _BASE_SELECT_POLLER_H
#define _BASE_SELECT_POLLER_H

#include "base/Poller.h"

#ifdef __Win32__
#include <WinSock2.h>
#elif __linux__
#include <sys/select.h>
#include <sys/types.h>
#endif
#include "base/Mutex.h"

namespace base {



class SelectPoller : public Poller
{
public:
	SelectPoller();
	virtual ~SelectPoller() {}
	
	virtual	int AddEvent(int fd, int event, EventCallback callback);
	virtual int ModifyEvent(int fd, int type);
	virtual int RemoveEvent(int fd);
	virtual Result Poll();
	virtual Result PollTime(int64_t usecond);

private:	
	fd_set readSet;
	fd_set writeSet;
	fd_set excptionSet;
	base::Mutex mutex_;

	std::vector<std::shared_ptr<EventBase>> callbackList_;
};
	

	
}

#endif
