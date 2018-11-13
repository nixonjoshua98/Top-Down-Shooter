#ifndef JN_TIMER_H
#define JN_TIMER_H

#include <string>
#include <ctime>

class JN_Timer
{
private:
	int frameCount;
	int startTicks;
	int FPS;
	int actualFPS;

public:
	JN_Timer(int fps);

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

#endif //!JN_TIMER_H