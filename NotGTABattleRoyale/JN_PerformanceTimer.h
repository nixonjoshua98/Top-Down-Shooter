#ifndef JN_PERFORMANCETIMER_H
#define JN_PERFORMANCETIMER_H

#include <string>
#include <ctime>

class JN_PerformanceTimer
{
private:
	int frameCount;
	int startTicks;
	int FPS;
	int actualFPS;

public:
	JN_PerformanceTimer();


	JN_PerformanceTimer(int fps);

	void Tick();
	void Wait();

	int GetFrameCount()
	{
		return frameCount;
	}

	int GetFPS()
	{
		return actualFPS;
	}

	int GetAimFPS()
	{
		return FPS;
	}

	std::tm* GetTime()
	{
		time_t t = std::time(0);
		return std::localtime(&t);
	}
};

#endif //!JN_PERFORMANCETIMER_H