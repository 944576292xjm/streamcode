#ifndef _BASE_TIME_TICK_H
#define _BASE_TIME_TICK_H

#include <stdint.h>

namespace base {
	
	
class TimeTick
{
public:
	TimeTick() : ticks_(0) {}

	explicit TimeTick(int64_t tick) : ticks_(tick) {}

	static TimeTick Now(); 
	static int64_t GetMonotonicNow();

	bool IsValid() {return ticks_ > 0;}

	/*TimeTick& operator=(TimeTick other)
	{
		ticks_ = other.ticks_;
		return *this;
	}*/

	int64_t operator()() const
	{
		return ticks_;
	}

	int64_t operator+(TimeTick other) const
	{
		return ticks_ + other.ticks_;
	}
	int64_t operator-(TimeTick other) const
	{
		return ticks_ - other.ticks_;
	}

	bool operator==(TimeTick other) const
	{
		return ticks_ == other.ticks_;
	}

	bool operator<(TimeTick other) const
	{
		return ticks_ < other.ticks_;
	}

	bool operator<=(TimeTick other) const
	{
		return ticks_ <= other.ticks_;
	}

	bool operator>(TimeTick other) const
	{
		return ticks_ > other.ticks_;
	}

	bool operator>=(TimeTick other) const
	{
		return ticks_ >= other.ticks_;
	}

private:

	int64_t ticks_;	
};
	
	
	
	
	
}


#endif