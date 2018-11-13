#ifndef JN_REAL_TIMER_H
#define JN_REAL_TIMER_H

#include <chrono>
#include <string>

class JN_RealTimer
{
public:
	JN_RealTimer()
	{
		start = std::chrono::system_clock::now();
	}

	float Tick()
	{
		auto end = std::chrono::system_clock::now();
		std::chrono::duration<float, std::milli> span = end - start;

		return span.count();
	}

	static std::string GetTime()
	{
		time_t t = time(0);
		std::tm* now = localtime(&t);

		return std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec);
	}

private:
	std::chrono::system_clock::time_point start;
};


#endif // !JN_REAL_TIMER_H
