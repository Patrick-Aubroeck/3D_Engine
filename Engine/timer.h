#ifndef _TIMER_H_
#define _TIMER_H_

class Timer
{
private:
	INT64 m_frequency; // holds the frequency
	float m_ticksPerMs; // holds the ticks per milisecond
	INT64 m_startTime; // holds the startime
	float m_frameTime; // holds the frametime
public:
	Timer(); // standard constructor
	~Timer(); //standard destructor
	bool Initialize(); //initialize the timer object
	void Frame(); //set the private variable(only call this after initialize has been called before.
	float GetFrameTime(); //return the frametime
	void Shutdown();
};

#endif