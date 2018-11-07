#include "stdafx.h"

#include "JN_Timer.h"

#include "SDL.h"

JN_Timer::JN_Timer(int fps)
{
	this->FPS = fps;
}

void JN_Timer::Tick()
{
	this->startTicks = SDL_GetTicks();
	frameCount++;
}

void JN_Timer::Wait()
{
	int nowTicks = SDL_GetTicks();

	if ((1000 / this->FPS) > (nowTicks - startTicks))
		SDL_Delay((1000 / this->FPS) - (nowTicks - startTicks));
}