#ifndef JN_PERFORMANCETIMER_H
#define JN_PERFORMANCETIMER_H

#include <string>
#include <ctime>

class JN_PerformanceTimer
{
public:
	JN_PerformanceTimer();


	JN_PerformanceTimer(int fps);


	void Tick();


	void Wait();


	int GetFrameCount();


	int GetFPS();


	std::tm* GetTime();

private:
	int frameCount;
	int startTicks;
	int FPS;
	int actualFPS;
};

#endif //!JN_PERFORMANCETIMER_H