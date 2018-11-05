#include "stdafx.h"

#include "JN_GameWorld.h"
#include "JN_Logging.h"

#include <iostream>
#include <fstream>
#include <string>

JN_GameWorld::JN_GameWorld()
{
	LoadWorldFile();
}

JN_GameWorld::~JN_GameWorld()
{
	for (auto s : emptyTiles)
		delete s;

	for (auto s : collisionTiles)
		delete s;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool JN_GameWorld::Init()
{
	JN_Log("GameWorld", "Init", false);

	bool success = SDL_Init(SDL_INIT_EVERYTHING) == 0;

	if (success) {
		window = SDL_CreateWindow("Not Grand Theft Auto Battle Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == NULL)
			success =  false;
		else
		{
			renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

			player.Init(renderer);
			BuildWorld();
		}
	}

	return success;
}

void JN_GameWorld::Run()
{
	JN_Log("GameWorld", "Game loop started", false);

	while (running)
	{
		timer.Tick();

		Input();
		Update();
		LateUpdate();
		Render();

		timer.Wait();
	}
}

void JN_GameWorld::Input()
{
	JN_Log("GameWorld", "Input", false);

	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:
			running = false;
			break;

		default:
			player.Input(e);
			break;
		}
	}
}

void JN_GameWorld::Render()
{
	JN_Log("GameWorld", "Render", false);

	SDL_RenderClear(renderer);	// Clear render

	for (int i = 0; i < (int)collisionTiles.size(); i++)
		collisionTiles[i]->Render(renderer);

	for (int i = 0; i < (int)emptyTiles.size(); i++)
		emptyTiles[i]->Render(renderer);


	player.Render(renderer);

	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 0);	// Set background color
	SDL_RenderPresent(renderer);						// Flip the render
}

void JN_GameWorld::Update()
{
	JN_Log("GameWorld", "Update", false);

	player.Update();
}

void JN_GameWorld::LateUpdate()
{
	JN_Log("GameWorld", "LateUpdate", false);

	player.LateUpdate(collisionTiles);
}

void JN_GameWorld::LoadWorldFile()
{
	std::ifstream file("Data/World.txt");

	if (file.bad())
		return;

	std::string line;

	int i = 0;
	while (std::getline(file, line))
	{
		for (char cell : line)
		{
			if (i + 1 >= LEVEL_SIZE)
				break;

			charWorldArray[i++] = cell;
		}

		if (i + 1 >= LEVEL_SIZE)
			break;
	}

	file.close();
}

void JN_GameWorld::BuildWorld()
{
	JN_Sprite *s;
	SDL_Rect r;

	r.w = CELL_WIDTH;
	r.h = CELL_HEIGHT;

	for (int i = 0; i < LEVEL_HEIGHT; i++)
	{
		for (int j = 0; j < LEVEL_WIDTH; j++)
		{
			s = new JN_Sprite();

			r.x = j * CELL_WIDTH;
			r.y = (i * CELL_HEIGHT) + BANNER_HEIGHT;

			switch (charWorldArray[(i * LEVEL_WIDTH) + j])
			{
			case JN_Sprite::EMPTY_TILE_CHAR:
				s->Init(JN_Sprite::SpriteType::EMPTY, renderer, r);
				emptyTiles.push_back(s);
				break;

			case JN_Sprite::MOVEMENT_DEBUFF_TILE_CHAR:
				s->Init(JN_Sprite::SpriteType::MOVEMENT_DEBUFF, renderer, r);
				collisionTiles.push_back(s);
				break;
			}
		}
	}
}