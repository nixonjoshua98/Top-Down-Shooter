#ifndef JN_LOGGING_H
#define JN_LOGGING_H

#include <string>
#include <vector>
#include <thread>


class JN_Queue
{
public:
	std::string Next()
	{
		std::string r;

		if (container.size() > 0)
		{
			r = container[0];
			container.erase(container.begin());
		}
		else
			r = "0";

		return r;
	}

	void Add(std::string s)
	{
		container.push_back(s);
	}

	int Size()
	{
		return (int)container.size();
	}

private:
	std::vector<std::string> container = {};
};


class JN_Logging
{
public:
	JN_Logging();
	~JN_Logging();

	JN_Queue logQueue = JN_Queue();

	void Log();

	void LogPerformance(int frameN, int currentFPS, int aimFPS);
	void LogTimeSpan(std::string txt, float time);
	void LogMethod(std::string txt);
	void LogWindowSize(int xa, int ya, int xb, int yb);
	void LogKeyboardInput(bool down, std::string key);

private:
	bool currentlyLogging = true;
	bool finishedLogging  = false;

	void ClearLog();
};

#endif // !JN_LOGGING_H
