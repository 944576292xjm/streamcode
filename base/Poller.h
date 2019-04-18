#ifndef _BASE_POLLER_H
#define _BASE_POLLER_H

#ifdef __Win32__
#include <Winsock2.h>
#elif __linux__

#endif
#include <functional>
#include <unordered_map>
#include "base/Callback.h"

namespace base {
	
enum 
{
	EventDummy = 1 << 0,
	EventRead = 1 << 1,
	EventWrite = 1 << 2,
	//EventClose = 1 << 3;
	EventError = 1 << 4,
};

struct EventBase
{
	EventBase(int fd, int event, EventCallback callback)
		: fd_(fd), event_(event_), callback_(std::move(callback)) {}
	void attachEvent(int event) { attach = event; }

	int fd_;
	int event_;
	int attach;
	EventCallback callback_;
};
	
class Poller
{
public:

	typedef enum
	{
		Error,
		TimedOut,
		ProcessIO,
	} Result;

	virtual	int AddEvent(int fd, int event, EventCallback callback) = 0;
	
	virtual int ModifyEvent(int fd, int type) = 0;
	
	virtual int RemoveEvent(int fd) = 0;
	
	virtual Result Poll() = 0;
	
	virtual Result PollTime(int64_t usecond) = 0;

protected:
	typedef std::unordered_map<int, std::shared_ptr<EventBase>> EventMap;
	EventMap  eventMap_;

	//EventMap taskMap_;

	
};


	
}

#endif