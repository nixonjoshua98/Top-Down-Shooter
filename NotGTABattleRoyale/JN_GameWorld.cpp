#include "stdafx.h"

#include "JN_GameWorld.h"
#include "JN_Logging.h"
#include "JN_RealTimer.h"

#include "JN_ReadWriteFunctions.h"

#include <iostream>
#include <SDL_ttf.h>
#include <string>
#include <math.h>

// Default constructor
JN_GameWorld::JN_GameWorld()
{
	logObj = new JN_Logging();	// THIS HAS TO STAY IN THE CONSTRUCTOR
}


// Deconstructor (De-allocates memory)
JN_GameWorld::~JN_GameWorld()
{
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

	logObj->Log();

	// Stop logging and remove pointers
	delete logObj;
	delete windowData;
	delete timerText;

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
		window = SDL_CreateWindow("Joshua Nixon, Games Computing (BSc), 16632283 | Not GTA Battle Royale", 
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

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

			Setup();
		}
	}

	return success;
}


void JN_GameWorld::Setup()
{
	timerText = new JN_Text();
	performanceTimer = JN_PerformanceTimer(FPS);
	gameplayTimer = JN_GameplayTimer();
	windowData = new JN_WindowData(0, 0, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);

	SDL_SetWindowMinimumSize(window, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);

	bool worldLoaded = ReadTextFile("Data/World.txt", charWorldArr, LEVEL_SIZE);

	if (worldLoaded)
		logObj->LogMethod("World text file was read OK");
	else
	{
		logObj->LogMethod("World text file failed to be read so a random map was generated");
		CreateRandomWorldMap();
	}

	BuildWorld();
}


// Game loop
void JN_GameWorld::Run()
{
	while (running && !timerComplete)
	{
		performanceTimer.Tick();	// Must be at the start of the loop

		logObj->LogPerformance(performanceTimer.GetFrameCount(), performanceTimer.GetFPS(), FPS);

		Input();

		if (!gamePaused)
		{
			Update();
			LateUpdate();
		}

		Render();

		logObj->Log();				// Appends the log queue to console and file
		performanceTimer.Wait();	// Waits for the needed time to match the FPS aim
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
			if (e.type == SDL_KEYDOWN && (e.key.keysym.scancode == FULL_SCREEN_KEY) && !gamePaused)
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
	{
		allTiles[i]->Render(renderer);
	}

	logObj->LogTimeSpan("World tiles finished rendering", t.Tick());

	t = JN_RealTimer();

	player.Render(renderer);

	logObj->LogTimeSpan("Player finished rendering", t.Tick());

	timerText->Render(renderer, std::to_string(60 - (int)gameDuration / 1000));

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);	// Set background color
	SDL_RenderPresent(renderer);					// Flip the render

	logObj->LogMethod("Renderer flipped");
}


// Call update on all the objects required
void JN_GameWorld::Update()
{
	gameplayTimer.Tick();

	gameDuration = gameplayTimer.GetDuration();

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


void JN_GameWorld::CreateRandomWorldMap()
{
	for (int i = 0; i < LEVEL_SIZE; i++)
		charWorldArr[i] = (char)(rand() % JN_Gameobject::TOTAL_TILES) + '0';
}


// Builds the world based on the char array
void JN_GameWorld::BuildWorld()
{
	JN_Gameobject::tileSheet.Init(renderer, "Assets/TileSheet.bmp", false, logObj);
	JN_Gameobject::playerProjectile.Init(renderer, "Assets/PlayerProjectile.bmp", true, logObj);
	JN_Gameobject::playerSpriteSheet.Init(renderer, "Assets/PlayerSpriteSheet.bmp", true, logObj);

	player.Init(renderer, logObj, windowData);
	timerText->Init((WORLD_WIDTH / 2) - 25, 10, 50, 50, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	JN_Gameobject *s;
	SDL_Rect r;

	r.w = CELL_WIDTH;
	r.h = CELL_HEIGHT;

	JN_RealTimer t = JN_RealTimer();

	for (int i = 0; i < LEVEL_HEIGHT; i++)
	{
		for (int j = 0; j < LEVEL_WIDTH; j++)
		{
			s = new JN_Gameobject();

			r.x = j * CELL_WIDTH;
			r.y = i * CELL_HEIGHT;

			switch (charWorldArr[(i * LEVEL_WIDTH) + j])
			{
			default:
				s->Init(JN_Gameobject::Tag::EMPTY, JN_Gameobject::tileSheet.GetTexture(), r, s->EMPTY_TILE_RECT, logObj);
				emptyTiles.push_back(s);
				break;

			case '1':
				s->Init(JN_Gameobject::Tag::MOVEMENT_DEBUFF, JN_Gameobject::tileSheet.GetTexture(), r, s->MOVEMENT_DEBUFF_TILE_RECT, logObj);
				collisionTiles.push_back(s);
				break;

			case '2':
				s->Init(JN_Gameobject::Tag::DAMAGE, JN_Gameobject::tileSheet.GetTexture(), r, s->DAMAGE_TILE_RECT, logObj);
				collisionTiles.push_back(s);
				break;

			}

			allTiles.push_back(s);
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
	timerText->Move(xChange, yChange);
	
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
	auto flag = fullscreen ? SDL_WINDOW_SHOWN : SDL_WINDOW_FULLSCREEN_DESKTOP;
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

	gameplayTimer.SetStartTime();

	gamePaused = !gamePaused;
}