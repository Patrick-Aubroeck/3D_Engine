#include <Windows.h> 

#include "timer.h"

/// <summary>standard constructor</summary>
Timer::Timer() :
	m_frameTime(0.0f),
	m_frequency(0),
	m_startTime(0),
	m_ticksPerMs(0.0f)
{

}

/// <summary>standard destructor</summary>
Timer::~Timer()
{

}

/// <summary> will shutdown the timer and call shutdown on al its member variable</summary>
void Timer::Shutdown()
{
	// nothing to do for now
}

/// <summary>initialize the timer</summary>
/// <param name ="identifier"> </param>
/// <returns> returns true if the system support high performance timers, false if not</returns>
/// <remark>will check if the system supports high performance timers, if the system supports it calculate the tickspermilisecond and get the starttime</remark>
/// <seealso cref="QueryPerformanceCounter"/>
bool Timer::Initialize()
{
	// Check to see if this system supports high performance timers.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_frequency);
	if(m_frequency == 0)
	{
		return false;
	}

	// Find out how many times the frequency counter ticks every millisecond.
	m_ticksPerMs = (float)(m_frequency / 1000);

	QueryPerformanceCounter((LARGE_INTEGER*)&m_startTime);

	return true;
}

/// <summary>set the starttime as the currenttime and calculates/set the frametime</summary>
/// <remark>the currenttime will be set by calling a queryperformancecounter</remark>
/// <remark>the timedifference will be set by getting the currenttime minus the starttime</remark>
/// <remark>the frametime is set by deviding the timedifference with the amount of ticspermilisecond </remark>
/// <remark>the starttime is reset to the currenttime </remark>
/// <seealso cref="QueryPerformanceCounter"/>
void Timer::Frame()
{
	INT64 currentTime;
	float timeDifference;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	timeDifference = (float)(currentTime - m_startTime);

	m_frameTime = timeDifference / m_ticksPerMs;

	m_startTime = currentTime;
}

/// <summary>get the frametime</summary>
/// <returns>returns the frametime as a float </returns>
float Timer::GetFrameTime()
{
	return m_frameTime;
}