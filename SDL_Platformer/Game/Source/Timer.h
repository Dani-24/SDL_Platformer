#ifndef __TIMER_H__
#define __TIMER_H__

#include "Defs.h"

class Timer
{
public:

	// Constructor
	Timer();

	void Start();
	void Stop();

	uint32 Read() const;
	float ReadSec() const;

private:
	uint32 startTime;
	uint stopTime;
	bool	running;
};

#endif //__TIMER_H__