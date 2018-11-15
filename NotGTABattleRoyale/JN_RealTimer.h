#ifndef JN_REAL_TIMER_H
#define JN_REAL_TIMER_H

#include <chrono>
#include <string>

class JN_RealTimer
{
public:
	///<summary>Default constructor</summary>
	JN_RealTimer();


	///<summary>Returns time since init in milliseconds</summary>
	float Tick();


	///<summary>Returns current time (H:M:S)
	static std::string GetTime();

protected:
	std::chrono::system_clock::time_point start;	// Time at init
};

#endif // !JN_REAL_TIMER_H
