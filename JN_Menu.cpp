
#include "stdafx.h"

#include <iostream>

#include "JN_Menu.h"
#include "JN_GameWorld.h"

bool JN_Menu::GetQuit()
{
	return isQuit;
}

void JN_Menu::Run(SDL_Renderer* renderer, std::vector<JN_GameObject*> tiles)
{
	allTiles = tiles;
	pTimer = JN_PerformanceTimer(JN_GameWorld::FPS);
	r = renderer;

	while (!isQuit)
	{
		pTimer.Tick();
		Input();
		Render();
		pTimer.Wait();
	}
}

void JN_Menu::Input()
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			isQuit = true;
			break;
		}
	}
}

void JN_Menu::Render()
{
	for (auto t : allTiles)
		t->Render(r);


	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);	// Set background color
	SDL_RenderPresent(r);					// Flip the render
}