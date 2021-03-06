#include "stdafx.h"

#include "JN_RealTimer.h"


// Default constructor
JN_RealTimer::JN_RealTimer()
{
	Reset();
}


// Returns time since start in milliseconds
float JN_RealTimer::Tick()
{
	auto end = std::chrono::system_clock::now();
	std::chrono::duration<float, std::milli> span = end - start;

	return span.count();
}

void JN_RealTimer::Reset()
{
	start = std::chrono::system_clock::now();
}


// Static method, returns current time H:M:S
std::string JN_RealTimer::GetTime()
{
	time_t t = time(0);
	std::tm* now = localtime(&t);

	return std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + ":" + std::to_string(now->tm_sec);
}