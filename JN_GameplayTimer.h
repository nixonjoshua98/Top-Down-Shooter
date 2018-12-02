#ifndef JN_GAMEPLAY_TIMER_H
#define JN_GAMEPLAY_TIMER_H

#include "JN_RealTimer.h"

class JN_GameplayTimer : public JN_RealTimer
{
public:
	void SetStartTime()
	{
		Reset();
	}


	void Reset()
	{
		start = std::chrono::system_clock::now();
	}


	float Tick()
	{
		// i would have used JN_RealTimer::Tick() but i was getting an error, perhaps becuase its a header-only class?
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<float, std::milli> span = end - start;

		duration += span.count();
		start = std::chrono::system_clock::now();

		return duration;
	}

	float GetDuration()
	{
		return duration;
	}

	float duration = 0.0f;
};

#endif // !JN_GAMEPLAY_TIMER_H
