#include "stdafx.h"

#include "JN_SplashScreen.h"
#include "JN_GameWorld.h"
#include "JN_RealTimer.h"

void JN_SplashScreen::Run(SDL_Renderer* renderer, std::vector<JN_GameObject*> tiles, int duration)
{
	this->timer = JN_PerformanceTimer(JN_GameWorld::FPS);

	this->allTiles = tiles;

	auto timer = JN_RealTimer();

	ReadJson("Data/controls.json", controls);

	speedControlsTxt = new JN_Text();
	titleTxt = new JN_Text();
	shootTxt = new JN_Text();
	movementControlsTxt = new JN_Text();
	toggleTxt = new JN_Text();
	pauseTxt = new JN_Text();
	fullscreenTxt = new JN_Text();

	titleTxt->Init((JN_GameWorld::MIN_WINDOW_WIDTH / 2) - 250, 25, 500, 100, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	movementControlsTxt->Init(50, 125, 600, 100, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	speedControlsTxt->Init   (50, 225, 600, 100, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	shootTxt->Init           (50, 325, 600, 100, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	toggleTxt->Init          (50, 425, 600, 100, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	fullscreenTxt->Init      (50, 525, 600, 100, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	pauseTxt->Init           (50, 625, 600, 100, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	while (duration - (timer.Tick() / 1000) > 0 && (!isQuit))
	{
		this->timer.Tick();
		Input();
		Render(renderer);
		this->timer.Wait();
	}

	delete titleTxt, movementControlsTxt, speedControlsTxt, shootTxt, toggleTxt, fullscreenTxt;
}


void JN_SplashScreen::Render(SDL_Renderer* renderer)
{
	for (auto t : allTiles)
		t->Render(renderer);

	titleTxt->Render(renderer, "Not GTA Battle Royale");

	std::string movementControlsStr = "Movement controls >> ";
	movementControlsStr += controls["up"].get<std::string>() + ", ";
	movementControlsStr += controls["left"].get<std::string>() + ", ";
	movementControlsStr += controls["down"].get<std::string>() + ", ";
	movementControlsStr += controls["right"].get<std::string>();

	std::string speedControlsStr = "Speed controls >> ";
	speedControlsStr += controls["speedUp"].get<std::string>() + ", ";
	speedControlsStr += controls["speedDown"].get<std::string>();

	movementControlsTxt->Render(renderer, movementControlsStr);
	speedControlsTxt->Render(renderer, speedControlsStr);
	shootTxt->Render(renderer, "Shoot controls >> left Click");
	toggleTxt->Render(renderer, "Toggle logging >>> F1");
	fullscreenTxt->Render(renderer, "Fullscreen >>> F11");
	pauseTxt->Render(renderer, "Pause >>> ESC");

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