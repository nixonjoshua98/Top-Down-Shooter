#include "stdafx.h"

#include "JN_PerformanceTimer.h"

#include <SDL.h>


JN_PerformanceTimer::JN_PerformanceTimer()
{

}


JN_PerformanceTimer::JN_PerformanceTimer(int fps)
{
	frameCount = 0;
	this->FPS = fps;
}


int JN_PerformanceTimer::GetFrameCount()
{
	return frameCount;
}


int JN_PerformanceTimer::GetFPS()
{
	return actualFPS;
}


std::tm* JN_PerformanceTimer::GetTime()
{
	time_t t = std::time(0);
	return std::localtime(&t);
}


void JN_PerformanceTimer::Tick()
{
	JN_PerformanceTimer::frameCount++;

	actualFPS = (1000 / (SDL_GetTicks() - startTicks));
	startTicks = SDL_GetTicks();
}

void JN_PerformanceTimer::Wait()
{
	int nowTicks = SDL_GetTicks();

	if ((1000 / FPS) > (nowTicks - startTicks))
		SDL_Delay((1000 / FPS) - (nowTicks - startTicks));
}