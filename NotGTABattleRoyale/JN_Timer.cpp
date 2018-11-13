#include "stdafx.h"

#include "JN_Timer.h"

#include "SDL.h"

JN_Timer::JN_Timer(int fps)
{
	frameCount = 0;
	this->FPS = fps;
}

void JN_Timer::Tick()
{
	JN_Timer::frameCount++;

	actualFPS = (1000 / (SDL_GetTicks() - startTicks));
	startTicks = SDL_GetTicks();
}

void JN_Timer::Wait()
{
	int nowTicks = SDL_GetTicks();

	if ((1000 / this->FPS) > (nowTicks - startTicks))
		SDL_Delay((1000 / this->FPS) - (nowTicks - startTicks));
}