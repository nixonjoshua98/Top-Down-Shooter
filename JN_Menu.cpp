
#include "stdafx.h"

#include <iostream>

#include "JN_Menu.h"
#include "JN_GameWorld.h"

bool JN_Menu::GetQuit()
{
	return isQuit;
}

void JN_Menu::Init()
{
	startBtn = new JN_Button();
	quitBtn = new JN_Button();
	settingsBtn = new JN_Button();


	startBtn->Init("Start Round", (JN_GameWorld::WORLD_WIDTH / 2) - 200, (JN_GameWorld::WORLD_HEIGHT * 0.25f) - 75, 400, 150, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	settingsBtn->Init("Settings", (JN_GameWorld::WORLD_WIDTH / 2) - 200, (JN_GameWorld::WORLD_HEIGHT * 0.50f) - 75, 400, 150, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	quitBtn->Init("Quit Game"   , (JN_GameWorld::WORLD_WIDTH / 2) - 200, (JN_GameWorld::WORLD_HEIGHT * 0.75f) - 75, 400, 150, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
}

JN_Menu::~JN_Menu()
{
	delete startBtn, quitBtn, settingsBtn;
}

void JN_Menu::Reset()
{
	startBtn->Reset();
	quitBtn->Reset();
	settingsBtn->Reset();
}

void JN_Menu::Run(SDL_Renderer* renderer, std::vector<JN_GameObject*> tiles)
{
	allTiles = tiles;
	pTimer = JN_PerformanceTimer(JN_GameWorld::FPS);
	r = renderer;

	Init();

	while ((!isQuit) && (!isFinished))
	{
		pTimer.Tick();
		Reset();
		Input();
		Update();
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

		case SDL_MOUSEMOTION:
			startBtn->Input(e);
			quitBtn->Input(e);
			settingsBtn->Input(e);
			break;

		case SDL_MOUSEBUTTONDOWN:
			startBtn->Input(e);
			quitBtn->Input(e);
			settingsBtn->Input(e);
			break;

		default:
			break;
		}
	}
}

void JN_Menu::Update()
{
	startBtn->Update();
	quitBtn->Update();
	settingsBtn->Update();
	
	isQuit     = (isQuit || quitBtn->IsClicked());
	isFinished = startBtn->IsClicked();
}

void JN_Menu::Render()
{
	for (auto t : allTiles)
		t->Render(r);

	startBtn->Render(r);
	quitBtn->Render(r);
	settingsBtn->Render(r);


	SDL_SetRenderDrawColor(r, 0, 0, 0, 0);	// Set background color
	SDL_RenderPresent(r);					// Flip the render
}