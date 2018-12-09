#include "stdafx.h"

#include "JN_SplashScreen.h"
#include "JN_GameWorld.h"
#include "JN_RealTimer.h"

void JN_SplashScreen::Run(SDL_Renderer* renderer, std::vector<JN_GameObject*> tiles, int duration)
{
	this->allTiles = tiles;

	auto timer = JN_RealTimer();

	ReadJson("Data/controls.json", controls);

	speedControlsTxt = new JN_Text();
	titleTxt = new JN_Text();
	shootTxt = new JN_Text();
	movementControlsTxt = new JN_Text();

	titleTxt->Init(
		(int)(JN_GameWorld::WORLD_WIDTH * 0.5f) - 250, (int)(JN_GameWorld::WORLD_HEIGHT * 0.15f) - 50, 
		500, 100, 
		SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	movementControlsTxt->Init(50, (int)(JN_GameWorld::WORLD_HEIGHT * 0.4f) - 50,
		500, 100,
		SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	speedControlsTxt->Init(50, (int)(JN_GameWorld::WORLD_HEIGHT * 0.6f) - 50,
		500, 100,
		SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	shootTxt->Init(50, (int)(JN_GameWorld::WORLD_HEIGHT * 0.8f) - 50,
		500, 100,
		SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);


	while (duration - (timer.Tick() / 1000) > 0 && (!isQuit))
	{
		Input();
		Render(renderer);
	}

	delete titleTxt, movementControlsTxt, speedControlsTxt, shootTxt;

}


void JN_SplashScreen::Render(SDL_Renderer* renderer)
{
	for (auto t : allTiles)
		t->Render(renderer);

	titleTxt->Render(renderer, "Not GTA Battle Royale");

	std::string movementControlsStr = "Movement Controls: ";
	movementControlsStr += controls["up"].get<std::string>() + ", ";
	movementControlsStr += controls["left"].get<std::string>() + ", ";
	movementControlsStr += controls["down"].get<std::string>() + ", ";
	movementControlsStr += controls["right"].get<std::string>();

	std::string speedControlsStr = "Speed Controls: ";
	speedControlsStr += controls["speedUp"].get<std::string>() + ", ";
	speedControlsStr += controls["speedDown"].get<std::string>();

	movementControlsTxt->Render(renderer, movementControlsStr);
	speedControlsTxt->Render(renderer, speedControlsStr);
	shootTxt->Render(renderer, "Shoot Controls: Left Click");

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);	// Set background color
	SDL_RenderPresent(renderer);					// Flip the render
}

void JN_SplashScreen::Input()
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