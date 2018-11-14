#include "stdafx.h"

#include "JN_GameWorld.h"
#include "JN_Logging.h"
#include "JN_RealTimer.h"

#include <iostream>
#include <fstream>
#include <string>

// Default constructor
JN_GameWorld::JN_GameWorld()
{
	logObj = new JN_Logging();
	windowData = new JN_WindowData(0, 0, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);

	LoadWorldFile();
}


// Deconstructor (De-allocates memory)
JN_GameWorld::~JN_GameWorld()
{
	logObj->Log();

	for (int i = 0; i < (int)allTiles.size(); i++)
	{
		// Delete the tile from memory
		delete allTiles[i];
		allTiles[i] = NULL;

		// Remove the null pointer to the deleted tile
		if (i < (int)collisionTiles.size())
			collisionTiles[i] = NULL;

		if (i < (int)emptyTiles.size())
			emptyTiles[i] = NULL;
	}

	// Stop logging and remove the window data object
	delete logObj;
	delete windowData;

	// Destroy the window, renderer and stop all SDL subsystems
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	window = NULL;
	renderer = NULL;

	TTF_Quit();
	SDL_Quit();
}


// Initializes the window, renderer, font and starts to build the world
bool JN_GameWorld::Init()
{
	bool success = (SDL_Init(SDL_INIT_EVERYTHING) == 0) && (TTF_Init() == 0);	// Init SDL n TTF

	if (success) {
		window = SDL_CreateWindow("Joshua Nixon, Games Computing (BSc), 16632283 | Not GTA Battle Royale", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		if (window == NULL)
		{
			success = false;
			logObj->LogMethod("SDL window failed to be initialized");
		}
		else
		{
			logObj->LogMethod("SDL window initialized correctly");

			renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

			logObj->LogMethod("SDL renderer initialized correctly");

			SDL_SetWindowMinimumSize(window, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
			
			BuildWorld();
		}
	}

	return success;
}


// Game loop
void JN_GameWorld::Run()
{
	while (running)
	{
		timer.Tick();	// Must be at the start of the loop

		logObj->LogPerformance(timer.GetFrameCount(), timer.GetFPS(), timer.GetAimFPS());

		Input();

		if (!gamePaused)
		{
			Update();
			LateUpdate();
			Render();
		}

		logObj->Log();	// Appends the log queue to console and file

		timer.Wait();	// Waits for the needed time to match the FPS aim
	}

	logObj->LogMethod("Game loop exited");
}


// Calls all objects with input methods
void JN_GameWorld::Input()
{
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{
		switch (e.type)
		{
		case SDL_QUIT:	// Window has been closed
			running = false;
			logObj->LogMethod("Window was closed by the user");
			break;

		case SDL_WINDOWEVENT:
			// Window has been resized
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				JN_RealTimer t = JN_RealTimer();
				ResizeWorld();
				logObj->LogTimeSpan("Window resizing callback completed", t.Tick());
			}
			break;

		default:
			if (e.type == SDL_KEYDOWN && (e.key.keysym.scancode == FULL_SCREEN_KEY))
				ToggleFullScreen();

			else if (e.type == SDL_KEYDOWN && (e.key.keysym.scancode == LOG_TOGGLE_KEY))
				logObj->ToggleLogging();

			else if (e.type == SDL_KEYDOWN && (e.key.keysym.scancode == PAUSE_GAME_KEY))
				TogglePauseGame();

			else if (!gamePaused)
			{
				// Pass all other input to the player
				JN_RealTimer t = JN_RealTimer();
				player.Input(e);
				logObj->LogTimeSpan("Player input event check completed", t.Tick());
				break;
			}
		}
	}
}


// Render all of the objects
void JN_GameWorld::Render()
{
	SDL_RenderClear(renderer);	// Clear render

	JN_RealTimer t = JN_RealTimer();

	for (int i = 0; i < (int)allTiles.size(); i++)
		allTiles[i]->Render(renderer);

	logObj->LogTimeSpan("World tiles finished rendering", t.Tick());

	t = JN_RealTimer();

	player.Render(renderer);

	logObj->LogTimeSpan("Player finished rendering", t.Tick());

	SDL_SetRenderDrawColor(renderer, 155, 155, 155, 0);			// Set background color
	SDL_RenderPresent(renderer);								// Flip the render

	logObj->LogMethod("Renderer flipped");
}


// Call update on all the objects required
void JN_GameWorld::Update()
{
	JN_RealTimer t = JN_RealTimer();
	player.Update();
	logObj->LogTimeSpan("Player update method concluded", t.Tick());
}


// Call late update on all required objects
void JN_GameWorld::LateUpdate()
{
	JN_RealTimer t = JN_RealTimer();
	player.LateUpdate(collisionTiles);
	logObj->LogTimeSpan("Player late update method concluded", t.Tick());
}


// Loads in the world text file or randomly creates the world
void JN_GameWorld::LoadWorldFile()
{
	std::ifstream f("Data/World.txt");

	JN_RealTimer t = JN_RealTimer();

	if (!f.is_open())
	{
		logObj->LogMethod("World text file failed to be read");

		for (int i = 0; i < LEVEL_SIZE; i++)
		{
			char c = (rand() % JN_Sprite::TOTAL_TILE_TYPES) +  '0';
			charWorldArray[i] = c;
		}
	}
	else
	{
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

	logObj->LogTimeSpan("World representation created", t.Tick());
}


// Builds the world based on the char array
void JN_GameWorld::BuildWorld()
{
	JN_RealTimer t = JN_RealTimer();

	player.Init(renderer, logObj, windowData);

	logObj->LogTimeSpan("Player initialized", t.Tick());

	JN_Sprite *s;
	SDL_Rect r;

	r.w = CELL_WIDTH;
	r.h = CELL_HEIGHT;

	t = JN_RealTimer();

	for (int i = 0; i < LEVEL_HEIGHT; i++)
	{
		for (int j = 0; j < LEVEL_WIDTH; j++)
		{
			s = new JN_Sprite();

			r.x = j * CELL_WIDTH;
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
				logObj->LogMethod("Non-standard tile char found, replacing with empty tile");
				s->Init(JN_Sprite::SpriteType::EMPTY, renderer, r, logObj, 1);
				emptyTiles.push_back(s);
				break;
			}
		}
	}

	logObj->LogTimeSpan("World tiles initialized", t.Tick());
}


// Callback to resize the world in-scale with the window
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


// Toggles fullscreen mode
void JN_GameWorld::ToggleFullScreen()
{
	logObj->LogMethod("Window was made fullscreen");

	auto flag = fullscreen ? SDL_WINDOW_SHOWN : SDL_WINDOW_FULLSCREEN;
	SDL_SetWindowFullscreen(window, flag);
	fullscreen = !fullscreen;
}


// Toggles pause mode
void JN_GameWorld::TogglePauseGame()
{
	player.EmptyInput();

	if (gamePaused)
		logObj->LogMethod("Game was unpaused");
	else
		logObj->LogMethod("Game was paused");

	gamePaused = !gamePaused;


}


/*
void JN_GameWorld::RenderGameTimer()
{
	SDL_Color col = SDL_Color{ 0, 0, 0 };
	TTF_Font *font = TTF_OpenFont("Assets/321impact.ttf", 24);

	SDL_Surface *textSurface = TTF_RenderText_Solid(font, "Poop", col);
	SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	SDL_Rect r;
	r.x = windowData->xOffset + (windowData->windowWidth / 2);
	r.y = 5;
	r.w = 100;
	r.h = 50;

	SDL_RenderCopy(renderer, textTexture, NULL, &r);
}
*/