
#include "stdafx.h"

#include "JN_Logging.h"
#include "JN_RealTimer.h"

#include <iostream>
#include <fstream>
#include <sstream>


JN_Logging::JN_Logging()
{
	ClearLog();
	LogMethod("Log was cleared");
}


JN_Logging::~JN_Logging()
{
	
}


void JN_Logging::Log(bool logOverride)
{
	if (!currentlyLogging && !logOverride)
		return;

	std::string t;
	std::ofstream f;

	f.open("Data/Log.log", std::ios_base::app);

	while (logQueue.Size() > 0)
	{
		t = logQueue.Next();

		if (t == "0")
			continue;

		//std::cout << t << "\n";
		f << t << "\n";
	}

	f.close();
}


void JN_Logging::ClearLog()
{
	std::ofstream f;

	f.open("Data/Log.log", std::ios_base::trunc | std::ios_base::out);

	if (f.is_open())
		f.close();
}


void JN_Logging::ToggleLogging()
{
	if (currentlyLogging)
		logQueue.Add(JN_RealTimer::GetTime() + " [INPUT] " + "Log turned off");
	else
		logQueue.Add(JN_RealTimer::GetTime() + " [INPUT] " + "Log turned on");

	this->Log(true);

	currentlyLogging = !currentlyLogging;
}


void JN_Logging::LogPerformance(int frameN, int currentFPS, int aimFPS)
{
	if (!currentlyLogging)
		return;

	std::ostringstream oss;
	oss << JN_RealTimer::GetTime() << " [SYSTEM] " << "Frame: " << frameN << " | " << "FPS: " << currentFPS << " | " << "Optimal FPS: " << aimFPS;
	logQueue.Add(oss.str());
}


void JN_Logging::LogTimeSpan(std::string txt, float time)
{
	if (!currentlyLogging)
		return;

	std::ostringstream oss;
	oss << JN_RealTimer::GetTime() << " [SYSTEM] " << txt << " | " << "Duration (ms): " << time;
	logQueue.Add(oss.str());
}


void JN_Logging::LogMethod(std::string txt)
{
	if (!currentlyLogging)
		return;

	std::ostringstream oss;
	oss << JN_RealTimer::GetTime() << " [SYSTEM] " << txt;
	logQueue.Add(oss.str());
}


void JN_Logging::LogWindowSize(int xa, int ya, int xb, int yb)
{
	if (!currentlyLogging)
		return;

	std::ostringstream oss;
	oss << JN_RealTimer::GetTime() << " [INPUT] Window resized (" << xa << ", " << ya << ") -> " << "(" << xb << ", " << yb << ")";
	logQueue.Add(oss.str());

}


void JN_Logging::LogKeyboardInput(bool down, std::string key)
{
	if (!currentlyLogging)
		return;

	std::ostringstream oss;
	oss << JN_RealTimer::GetTime() << " [INPUT] Key " << (down ? "pressed" : "released") << " [" << key << "]";
	logQueue.Add(oss.str());
}


void JN_Logging::LogMouseInput(bool down, std::string key)
{
	if (!currentlyLogging)
		return;

	std::ostringstream oss;
	oss << JN_RealTimer::GetTime() << " [INPUT] Mouse " << (down ? "pressed" : "released") << " [" << key << "]";
	logQueue.Add(oss.str());
}
