#ifndef JN_LOGGING_H
#define JN_LOGGING_H

#include "JN_Queue.h"


class JN_Logging
{
public:
	JN_Logging();
	~JN_Logging();

	JN_Queue logQueue = JN_Queue();

	void Log(bool logOverride = false);

	void ToggleLogging();

	void LogPerformance(int frameN, int currentFPS, int aimFPS);
	void LogTimeSpan(std::string txt, float time);
	void LogMethod(std::string txt);
	void LogWindowSize(int xa, int ya, int xb, int yb);
	void LogKeyboardInput(bool down, std::string key);
	void LogMouseInput(bool down, std::string key);
	void LogVelocity(float x, float y);
	void LogGamepadInput(std::string b);


private:
	bool currentlyLogging = true;

	void ClearLog();
};

#endif // !JN_LOGGING_H
