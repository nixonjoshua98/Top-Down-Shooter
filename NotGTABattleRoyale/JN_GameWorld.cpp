#include "stdafx.h"

#include "JN_GameWorld.h"
#include "JN_RealTimer.h"
#include "JN_Logging.h"

#include <iostream>
#include <fstream>
#include <string>

JN_GameWorld::JN_GameWorld()
{
	LoadWorldFile();

	windowData = new JN_WindowData(0, 0, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
}

JN_GameWorld::~JN_GameWorld()
{
	for (int i = 0; i < (int)allTiles.size(); i++)
	{
		delete allTiles[i];
		allTiles[i] = NULL;

		if (i < (int)collisionTiles.size())
			collisionTiles[i] = NULL;

		if (i < (int)emptyTiles.size())
			emptyTiles[i] = NULL;
	}

	delete logObj;
	delete windowData;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool JN_GameWorld::Init()
{
	this->logObj = new JN_Logging();

	bool success = SDL_Init(SDL_INIT_EVERYTHING) == 0;

	if (success) {
		window = SDL_CreateWindow("Joshua Nixon, Games Computing (BSc), 16632283 | Not GTA Battle Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		if (window == NULL)
		{
			success = false;
			logObj->LogMethod("SDL window failed to be initialized");
		}
		else
		{
			renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

			SDL_SetWindowMinimumSize(window, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
			
			BuildWorld();
		}
	}

	return success;
}

void JN_GameWorld::Run()
{
	while (running)
	{
		timer.Tick();

		logObj->LogPerformance(timer.GetFrameCount(), timer.GetFPS(), timer.GetAimFPS());

		Input();
		Update();
		LateUpdate();
		Render();

		logObj->Log();

		timer.Wait();
	}
}

void JN_GameWorld::Input()
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;

		case SDL_WINDOWEVENT:
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				ResizeWorld();
			break;

		default:
			player.Input(e);
			break;
		}
	}
}

void JN_GameWorld::Render()
{
	SDL_RenderClear(renderer);	// Clear render

	for (int i = 0; i < (int)allTiles.size(); i++)
		allTiles[i]->Render(renderer);

	player.Render(renderer);

	SDL_SetRenderDrawColor(renderer, 155, 155, 155, 0);			// Set background color
	SDL_RenderPresent(renderer);								// Flip the render
}

void JN_GameWorld::Update()
{
	player.Update();
}

void JN_GameWorld::LateUpdate()
{
	player.LateUpdate(collisionTiles);
}

void JN_GameWorld::LoadWorldFile()
{
	std::ifstream f("Data/World.txt");

	if (f.bad())
	{
		return;
	}

	std::string line;
	int i = 0;

	while (std::getline(f, line))
	{
		for (char cell : line)
		{
			if (i + 1 > LEVEL_SIZE) // gut feeling > instead of >=
				break;

			charWorldArray[i++] = cell;
		}

		if (i + 1 > LEVEL_SIZE)
			break;
	}

	f.close();
}

void JN_GameWorld::BuildWorld()
{
	player.Init(renderer, logObj, windowData);

	JN_Sprite *s;
	SDL_Rect r;

	r.w = CELL_WIDTH;
	r.h = CELL_HEIGHT;

	for (int i = 0; i < LEVEL_HEIGHT; i++)
	{
		for (int j = 0; j < LEVEL_WIDTH; j++)
		{
			s = new JN_Sprite();

			r.x = j * CELL_WIDTH ;
			r.y = (i * CELL_HEIGHT ) + BANNER_HEIGHT;

			allTiles.push_back(s);

			switch (charWorldArray[(i * LEVEL_WIDTH) + j])
			{
			case JN_Sprite::EMPTY_TILE_CHAR:
				s->Init(JN_Sprite::SpriteType::EMPTY, renderer, r, logObj,  1);
				emptyTiles.push_back(s);
				break;

			case JN_Sprite::MOVEMENT_DEBUFF_TILE_CHAR:
				s->Init(JN_Sprite::SpriteType::MOVEMENT_DEBUFF, renderer, r, logObj, 1);
				collisionTiles.push_back(s);
				break;

			case JN_Sprite::FIRE_DAMAGE_TILE_CHAR:
				s->Init(JN_Sprite::SpriteType::FIRE_DAMAGE, renderer, r, logObj, 1);
				collisionTiles.push_back(s);
				break;

			default: 
				break;
			}
		}
	}
}

void JN_GameWorld::ResizeWorld()
{
	int wW, wH;
	SDL_GetWindowSize(window, &wW, &wH);

	logObj->LogWindowSize(windowData->windowWidth, windowData->windowHeight, wW, wH);

	int xChange = (wW - windowData->windowWidth) / 2;
	int yChange = (wH - windowData->windowHeight) / 2;

	windowData->windowHeight = wH;
	windowData->windowWidth = wW;
	windowData->xOffset = (wW - MIN_WINDOW_WIDTH) / 2;
	windowData->yOffset = (wH - MIN_WINDOW_HEIGHT) / 2;;

	player.Resize(xChange, yChange);
	
	for (int i = 0; i < LEVEL_HEIGHT; i++)
	{
		for (int j = 0; j < LEVEL_WIDTH; j++)
			allTiles[(i * LEVEL_WIDTH) + j]->Resize(xChange, yChange);
	}
}