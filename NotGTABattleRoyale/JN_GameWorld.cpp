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
	for (auto t : allTiles)
		delete t;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool JN_GameWorld::Init()
{
	bool success = SDL_Init(SDL_INIT_EVERYTHING) == 0;

	if (success) {
		window = SDL_CreateWindow("Joshua Nixon, Games Computing (BSc), 16632283 | Not GTA Battle Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		if (window == NULL)
			success =  false;
		else
		{
			renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

			SDL_SetWindowMinimumSize(window, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
			
			player.Init(renderer);
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

		Input();
		Update();
		LateUpdate();
		Render();

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
	SDL_RenderClear(renderer);									// Clear render

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

			allTiles.push_back(s);

			switch (charWorldArray[(i * LEVEL_WIDTH) + j])
			{
			case JN_Sprite::EMPTY_TILE_CHAR:
				s->Init(JN_Sprite::SpriteType::EMPTY, renderer, r, 1);
				emptyTiles.push_back(s);
				break;

			case JN_Sprite::MOVEMENT_DEBUFF_TILE_CHAR:
				s->Init(JN_Sprite::SpriteType::MOVEMENT_DEBUFF, renderer, r, 1);
				collisionTiles.push_back(s);
				break;

			case JN_Sprite::FIRE_DAMAGE_TILE_CHAR:
				s->Init(JN_Sprite::SpriteType::FIRE_DAMAGE, renderer, r, 1);
				collisionTiles.push_back(s);
				break;

			default:
				s->Init(JN_Sprite::SpriteType::EMPTY, renderer, r, 1);
				emptyTiles.push_back(s);
				break;
			}
		}
	}
}

void JN_GameWorld::ResizeWorld()
{
	//return;

	int wW, wH;
	SDL_GetWindowSize(window, &wW, &wH);

	int cellWidth  = wW / LEVEL_WIDTH;
	int cellHeight = wH / LEVEL_HEIGHT;

	SDL_SetWindowSize(window, cellWidth * LEVEL_WIDTH, cellHeight * LEVEL_HEIGHT);

	currentWindowWidth  = cellWidth * LEVEL_WIDTH;
	currentWindowHeight = cellHeight * LEVEL_HEIGHT;

	//player.Resize(0, 0, 0, 0);

	for (int i = 0; i < LEVEL_HEIGHT; i++)
	{
		for (int j = 0; j < LEVEL_WIDTH; j++)
		{
			allTiles[(i * LEVEL_WIDTH) + j]->Resize(j * cellWidth, BANNER_HEIGHT + (i * cellHeight), cellWidth, cellHeight);
		}
	}

}