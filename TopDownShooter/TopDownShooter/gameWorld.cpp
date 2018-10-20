#include "stdafx.h"
#include "gameWorld.h"

#include <iostream>
#include <fstream>
#include <string>

/* Header File Includes:
	"square.h"
	"SDL.h"
	"player.h"
*/

GameWorld::GameWorld()
{
	player.Init(0, 0, RGB{ 0, 255, 0 });
	LoadWorldFile();
	BuildWorld();
}

GameWorld::~GameWorld()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

bool GameWorld::Init()
{
	bool success = SDL_Init(SDL_INIT_EVERYTHING) == 0;

	if (success) {
		window = SDL_CreateWindow("Top Down Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

		if (window == NULL)
		{
			return false;
		}
		else
		{
			renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
		}
	}

	return success;
}

void GameWorld::Run()
{
	int now     = 0;
	int current = 0;
	
	while (running)
	{
		now = SDL_GetTicks();

		Input();
		Update();
		LateUpdate();
		Render();

		current = SDL_GetTicks();

		if (DELTA_TIME > (current - now))
		{
			SDL_Delay(DELTA_TIME - (current - now));
		}
	}
}

void GameWorld::Input()
{
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

void GameWorld::Render()
{
	for (auto s : worldArr)
	{
		s.Render(renderer);
	}

	player.Render(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);	// Set background color
	SDL_RenderPresent(renderer);						// Flip the render
	SDL_RenderClear(renderer);							// Clear render
}

void GameWorld::Update()
{
	player.Update();
}

void GameWorld::LateUpdate()
{
	player.LateUpdate(worldArrArrPtr);
}

void GameWorld::LoadWorldFile()
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
			charWorldArray[i++] = cell;
		}
	}
}

void GameWorld::BuildWorld()
{
	Square s;

	for (int i = 0; i < LEVEL_HEIGHT; i++)
	{
		for (int j = 0; j < LEVEL_WIDTH; j++)
		{
			s = Square();

			switch (charWorldArray[(i * LEVEL_WIDTH) + j])
			{
			case 'S':
				s.Init(Square::SquareType::FLOOR, j * CELL_WIDTH, i * CELL_HEIGHT, RGB{ 0, 155, 0 });
				break;
			default:
				s.Init(Square::SquareType::WALL, j * CELL_WIDTH, i * CELL_HEIGHT, RGB{ 155, 0, 0 });
				break;
			}
			worldArr[(i * LEVEL_WIDTH) + j] = s;
		}
	}
}