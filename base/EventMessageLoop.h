#ifndef _BASE_EVENT_MESSAGE_LOOP_H
#define _BASE_EVENT_MESSAGE_LOOP_H


#include <functional>
#include <memory>
#include "base/MessageLoop.h"
#include "base/Poller.h"
#include "base/Socket.h"
#include "base/Callback.h"
#include "base/SocketPipe.h"
#include "base/Thread.h"

namespace base {
	
class EventMessageLoop : public MessageLoop
{
public:
	EventMessageLoop();
	virtual ~EventMessageLoop(){
		int a = 0;
	}

	std::unique_ptr<Poller> poller_;
	
	//Socket wakeupWaiter_;
	int AddEvent(int sockfd, int event, EventCallback callback);

	int ModifyEvent(int sockfd, int event);

	int RemoveEvent(int sockfd);

	virtual void RunLoop();

protected:
	virtual void ScheduleWork();
	
private:
	std::unique_ptr<TcpSocket> wakeup_;
	SocketPipe pipeWrap_;
	void HandleWakeUpRead();

};



}


#endif