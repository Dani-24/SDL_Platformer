// ----------------------------------------------------
// Fast timer with milisecons precision
// ----------------------------------------------------

#include "Timer.h"
#include "SDL\include\SDL_timer.h"
	
Timer::Timer()
{
	Start();
}

void Timer::Start()
{
	running = true;
	startTime = SDL_GetTicks();
}

void Timer::Stop()
{
	running = false;
	stopTime = SDL_GetTicks();
}

uint32 Timer::Read() const
{
	if (running == true)
	{
		return SDL_GetTicks() - startTime;
	}
	else
	{
		return stopTime - startTime;
	}
}

float Timer::ReadSec() const
{
	return (float)(SDL_GetTicks() - startTime) / 1000.0f;
}