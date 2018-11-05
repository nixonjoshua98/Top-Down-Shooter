#ifndef JN_TIMER_H
#define JN_TIMER_H

#include <string>

class JN_Timer
{
private:
	int frameCount = 0;
	unsigned int FPS = 0;
	unsigned int startTicks = 0;

public:
	JN_Timer(int fps);

	void Tick();
	void Wait();

	std::string GetFrameCount()
	{
		return std::to_string(frameCount);
	}
};

#endif //!JN_TIMER_H