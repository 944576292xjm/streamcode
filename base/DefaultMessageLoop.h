#ifndef _BASE_DEFAULT_MESSAGE_LOOP_H
#define _BASE_DEFAULT_MESSAGE_LOOP_H

#include "base/MessageLoop.h"
#include "base/WaitableEvent.h"


namespace base {

class DefaultMessageLoop : public MessageLoop	
{
public:	
	virtual void ScheduleWork();
	virtual void RunLoop();

private:
	base::WaitableEvent event_;
};
	
	
}

#endif