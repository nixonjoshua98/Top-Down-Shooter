#include "stdafx.h"

#include "JN_GameWorld.h"
#include "JN_Logging.h"
#include "JN_RealTimer.h"

#include "JN_ReadWriteFunctions.h"

#include <iostream>
#include <SDL_ttf.h>
#include <string>
#include <math.h>
#include <iostream>

Mix_Chunk* JN_GameWorld::buttonClick = NULL;
Mix_Music* JN_GameWorld::bgm = NULL;
bool JN_GameWorld::isSfxMute = false;

// Default constructor
JN_GameWorld::JN_GameWorld()
{
	logObj = new JN_Logging();	// THIS HAS TO STAY IN THE CONSTRUCTOR
}


// Deconstructor (De-allocates memory)
JN_GameWorld::~JN_GameWorld()
{
	delete player;

	player = NULL;

	JN_GameObject::CleanupAssets();

	// Remove all enemies from memory
	while (enemies.size() != 0)
	{
		delete enemies[0];
		enemies[0] = NULL;
		enemies.erase(enemies.begin());
	}

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
	delete scoreText;
	delete healthText;
	delete sfxDownBtn;

	// Destroy the window, renderer and stop all SDL subsystems
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	window = NULL;
	renderer = NULL;

	Mix_FreeChunk(JN_GameWorld::buttonClick);
	Mix_FreeChunk(oof);
	Mix_FreeChunk(coins);
	Mix_FreeMusic(JN_GameWorld::bgm);

	TTF_Quit();
	SDL_Quit();
	Mix_Quit();
}


// Initializes the window, renderer, font and starts to build the world
bool JN_GameWorld::Init()
{
	Mix_OpenAudio(44100, AUDIO_S16, 1, 4096);

	bool success = (SDL_Init(SDL_INIT_EVERYTHING) == 0) && (TTF_Init() == 0);	// Init SDL n TTF

	if (success) {
		window = SDL_CreateWindow("Joshua Nixon, Games Computing (BSc), 16632283 | Not GTA Battle Royale", 
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
			MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT, 
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

		if (window == NULL)
		{
			success = false;
			logObj->LogMethod("SDL window failed to be initialized");
		}
		else
		{
			SDL_SetWindowResizable(window, SDL_FALSE);

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
	resumeBtn = new JN_Button();
	timerText = new JN_Text();
	scoreText = new JN_Text();
	performanceTimer = JN_PerformanceTimer(FPS);
	gameplayTimer = JN_GameplayTimer();
	player = new JN_Player();
	healthText = new JN_Text();
	bgmVolTxt = new JN_Text();
	sfxVolTxt = new JN_Text();
	sfxDownBtn = new JN_Button();
	stfVolUpBtn = new JN_Button();
	bgmVolUpBtn = new JN_Button();
	stfVolUpBtn = new JN_Button();
	bgmVolDownBtn = new JN_Button();
	windowData = new JN_WindowData(0, 0, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);
	continueBtn = new JN_Button();

	SDL_SetWindowMinimumSize(window, MIN_WINDOW_WIDTH, MIN_WINDOW_HEIGHT);

	bool worldLoaded = ReadTextFile("Data/World.txt", charWorldArr, LEVEL_SIZE);

	if (worldLoaded)
		logObj->LogMethod("World text file was read OK");
	else
	{
		logObj->LogMethod("World text file failed to be read so a random map was generated");
		CreateRandomWorldMap();
	}

	// Init the assets
	JN_GameObject::tileSheet.Init(renderer, "Assets/TileSheet.bmp", false, logObj);
	JN_GameObject::playerProjectile.Init(renderer, "Assets/PlayerProjectile.bmp", true, logObj);
	JN_GameObject::playerSpriteSheet.Init(renderer, "Assets/PlayerSpriteSheet.bmp", true, logObj);
	JN_GameObject::scorpionSpriteSheet.Init(renderer, "Assets/ScorpionSpriteSheet.bmp", true, logObj);

	JN_GameWorld::buttonClick = Mix_LoadWAV("Assets/buttonClick.wav");
	oof = Mix_LoadWAV("Assets/oof.wav");
	coins = Mix_LoadWAV("Assets/coins.wav");
	JN_GameWorld::bgm = Mix_LoadMUS("Assets/backgroundMusic.wav");

	BuildWorld();
}


// Game loop
void JN_GameWorld::Run()
{
	gameDuration = 0;
	gameplayTimer.SetStartTime();


	SDL_SetWindowResizable(window, SDL_TRUE);

	while (running)
	{
		performanceTimer.Tick();	// Must be at the start of the loop

		logObj->Log();

		logObj->LogPerformance(performanceTimer.GetFrameCount(), performanceTimer.GetFPS(), FPS);

		Input();

		logObj->Log();

		if (gameStarted && (!timerComplete))
		{
			if (!gamePaused)
			{
				SpawnEnemy();

				Update();

				logObj->Log();

				LateUpdate();
			}
			else
			{
				if (!timerComplete)
				{
					logObj->LogMethod("Pause menu rendered");

					// Game is paused
					gameplayTimer.SetStartTime();
					resumeBtn->Update();
					stfVolUpBtn->Update();
					sfxDownBtn->Update();
					bgmVolUpBtn->Update();
					bgmVolDownBtn->Update();

					if (stfVolUpBtn->IsClicked())
					{
						logObj->LogMethod("SFX volume up button clicked");
						sfxVolume = fminf(100, sfxVolume + 10);
						Mix_Volume(-1, sfxVolume);
					}

					if (sfxDownBtn->IsClicked())
					{
						logObj->LogMethod("SFX volume down button clicked");
						sfxVolume = fmaxf(0, sfxVolume - 10);
						Mix_Volume(-1, sfxVolume);
					}

					if (bgmVolUpBtn->IsClicked())
					{
						logObj->LogMethod("BGM volume up button clicked");
						bgmVolume = fminf(100, bgmVolume + 10);
						Mix_VolumeMusic(bgmVolume);
					}

					if (bgmVolDownBtn->IsClicked())
					{
						logObj->LogMethod("BGM volume down button clicked");
						bgmVolume = fmaxf(0, bgmVolume - 10);
						Mix_VolumeMusic(bgmVolume);
					}

					sfxDownBtn->Reset();
					stfVolUpBtn->Reset();
					bgmVolUpBtn->Reset();
					bgmVolDownBtn->Reset();

					if (resumeBtn->IsClicked())
						TogglePauseGame();
				}
			}
		}
		else
		{
			continueBtn->Update();
			if (running)
				running = !continueBtn->IsClicked();
			continueBtn->Reset();				
		}

		Render();

		logObj->Log();				// Appends the log queue to console and file
		performanceTimer.Wait();	// Waits for the needed time to match the FPS aim
	}

	logObj->LogMethod("Game loop exited");
}


// Spawn an enemy if timer is finished
void JN_GameWorld::SpawnEnemy()
{
	if ((gameDuration - lastEnemySpawn) >= 333 && (enemies.size() < MAXIMUM_ENEMIES))
	{
		_++;
		lastEnemySpawn = gameDuration;
	
		JN_Enemy* e = new JN_Enemy();		

		int x = (windowData->xOffset) + (rand() % (WORLD_WIDTH));
		int y = (windowData->yOffset) + (rand() % (WORLD_HEIGHT));

		e->Init(JN_GameObject::Tag::SCORPION, SDL_Rect{ x,  y}, JN_GameObject::scorpionSpriteSheet.GetTexture(), logObj, windowData);

		enemies.push_back(e);

		logObj->LogMethod("Enemy spawned");
	}
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
			isQuit = true;
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

			else if (e.type == SDL_KEYDOWN && (e.key.keysym.scancode == PAUSE_GAME_KEY) && (gameStarted) && (!timerComplete))
				TogglePauseGame();

			else if ((!gameStarted || gamePaused) && (e.type == SDL_MOUSEBUTTONDOWN))
			{
				if (gamePaused)
				{
					bgmVolUpBtn->Input(e);
					sfxDownBtn->Input(e);
					bgmVolDownBtn->Input(e);
					stfVolUpBtn->Input(e);
					resumeBtn->Input(e);
				}
			}

			else if (!gamePaused && (gameStarted) && (!timerComplete))
			{
				// Pass all other input to the player
				JN_RealTimer t = JN_RealTimer();
				player->Input(e);
				logObj->LogTimeSpan("Player input event check completed", t.Tick());
				break;
			}

			else if (timerComplete)
			{
				continueBtn->Input(e);
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

	scoreText->Render(renderer, "SCORE: " + std::to_string(player->GetScore()));
	healthText->Render(renderer, "DMG TAKEN: " + std::to_string(player->GetDamageTaken()));

	if (!timerComplete)
	{
		for (int i = 0; i < (int)enemies.size(); i++)
			enemies[i]->Render(renderer);

		t = JN_RealTimer();

		player->Render(renderer);

		logObj->LogTimeSpan("Player finished rendering", t.Tick());

		timerText->Render(renderer, "TIMER: " + std::to_string(60 - (int)gameDuration / 1000));
	}

	if (gamePaused)
	{
		sfxDownBtn->Render(renderer);
		stfVolUpBtn->Render(renderer);
		bgmVolDownBtn->Render(renderer);
		bgmVolUpBtn->Render(renderer);
		bgmVolTxt->Render(renderer, "BGM: " + std::to_string(bgmVolume));
		resumeBtn->Render(renderer);
		sfxVolTxt->Render(renderer, "SFX: " + std::to_string(sfxVolume));
	}

	if (timerComplete)
		continueBtn->Render(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);	// Set background color
	SDL_RenderPresent(renderer);					// Flip the render

	logObj->LogMethod("Renderer flipped");
}


// Call update on all the objects required
void JN_GameWorld::Update()
{
	SDL_GameControllerUpdate();

	gameDuration = gameplayTimer.Tick();

	for (int i = 0; i < (int)enemies.size(); i++)
		enemies[i]->Update(player->rect);

	JN_RealTimer t = JN_RealTimer();
	player->Update();
	logObj->LogTimeSpan("Player update method concluded", t.Tick());

	timerComplete = timerComplete || gameDuration >= 60000;

	if (timerComplete)
	{
		scoreText->rect  = SDL_Rect{ (windowData->windowWidth / 2) - 120 , windowData->windowHeight / 2, 240, 50};
		healthText->rect = SDL_Rect{ (windowData->windowWidth / 2) - 120 , (windowData->windowHeight / 2) - 75, 240, 50 };
	}
}


// Call late update on all required objects
void JN_GameWorld::LateUpdate()
{
	JN_RealTimer t = JN_RealTimer();
	player->LateUpdate(collisionTiles, enemies);
	logObj->LogTimeSpan("Player late update method concluded", t.Tick());

	for (int i = 0; i < enemies.size();)
	{
		if (enemies[i]->isCollidingWithPlayer || enemies[i]->isDead)
		{
			if (enemies[i]->isCollidingWithPlayer)
			{
				if (!JN_GameWorld::isSfxMute)
					Mix_PlayChannel(-1, oof, 0);
				logObj->LogMethod("Player took 3 damage");
				player->TakeDamage(3);
			}
			else
			{
				if (!JN_GameWorld::isSfxMute)
					Mix_PlayChannel(-1, coins, 0);
				logObj->LogMethod("Player gained 5 score by killing an enemy");
				player->AddScore(5);
			}

			// TODO: Enemy Controller
			delete enemies[i];
			enemies[i] = NULL;
			enemies.erase(enemies.begin() + i);
			continue;
		}
		i++;
	}
}


void JN_GameWorld::CreateRandomWorldMap()
{
	for (int i = 0; i < LEVEL_SIZE; i++)
		charWorldArr[i] = (char)(rand() % JN_GameObject::TOTAL_TILES) + '0';
}


// Builds the world based on the char array
void JN_GameWorld::BuildWorld()
{
	player->Init(renderer, logObj, windowData);
	timerText->Init((int)(WORLD_WIDTH * 0.5f) - 120, 10, 240, 50, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	scoreText->Init((int)(WORLD_WIDTH * 0.8f) - 120, 10, 240, 50, SDL_Color{ 0, 0, 255 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	healthText->Init((int)(WORLD_WIDTH * 0.2f) - 120, 10, 240, 50, SDL_Color{ 255, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	sfxVolTxt->Init(                (WORLD_WIDTH / 2) - 75 , (WORLD_HEIGHT * 0.25f) + 175, 150,  40, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	resumeBtn->Init("Resume Round", (WORLD_WIDTH / 2) - 200, (WORLD_HEIGHT * 0.25f)      , 400, 150, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	sfxDownBtn->Init( "<", (WORLD_WIDTH / 2) - 160, (WORLD_HEIGHT * 0.25f) + 175, 40, 40, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	stfVolUpBtn->Init(">", (WORLD_WIDTH / 2) + 120, (WORLD_HEIGHT * 0.25f) + 175, 40, 40, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	bgmVolUpBtn->Init(">", (WORLD_WIDTH / 2) + 120, (WORLD_HEIGHT * 0.25f) + 240, 40, 40, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);
	bgmVolDownBtn->Init("<", (WORLD_WIDTH / 2) - 160, (WORLD_HEIGHT * 0.25f) + 240, 40, 40, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	bgmVolTxt->Init((WORLD_WIDTH / 2) - 75, (WORLD_HEIGHT * 0.25f) + 240, 150, 40, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	continueBtn->Init("Quit", (JN_GameWorld::WORLD_WIDTH / 2) - 200, (JN_GameWorld::WORLD_HEIGHT * 0.75f) - 75, 400, 150, SDL_Color{ 255, 255, 255 }, SDL_Color{ 0, 0, 0 }, "Assets/SourceSerifPro-Regular.ttf", 16);

	Mix_PlayMusic(JN_GameWorld::bgm, -1);

	Mix_Volume(-1, sfxVolume);
	Mix_VolumeMusic(bgmVolume);

	JN_GameObject *s;
	SDL_Rect r;

	r.w = CELL_WIDTH;
	r.h = CELL_HEIGHT;

	JN_RealTimer t = JN_RealTimer();

	for (int i = 0; i < LEVEL_HEIGHT; i++)
	{
		for (int j = 0; j < LEVEL_WIDTH; j++)
		{
			s = new JN_GameObject();

			r.x = j * CELL_WIDTH;
			r.y = i * CELL_HEIGHT;

			switch (charWorldArr[(i * LEVEL_WIDTH) + j])
			{
			default:
				s->Init(JN_GameObject::Tag::EMPTY, JN_GameObject::tileSheet.GetTexture(), r, s->EMPTY_TILE_RECT, logObj);
				emptyTiles.push_back(s);
				break;

			case '1':
				s->Init(JN_GameObject::Tag::MOVEMENT_DEBUFF, JN_GameObject::tileSheet.GetTexture(), r, s->MOVEMENT_DEBUFF_TILE_RECT, logObj);
				collisionTiles.push_back(s);
				break;

			case '2':
				s->Init(JN_GameObject::Tag::DAMAGE, JN_GameObject::tileSheet.GetTexture(), r, s->DAMAGE_TILE_RECT, logObj);
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

	player->Resize(xChange, yChange);
	timerText->Move(xChange, yChange);
	sfxVolTxt->Move(xChange, yChange);
	bgmVolUpBtn->Resize(xChange, yChange);
	bgmVolDownBtn->Resize(xChange, yChange);
	bgmVolTxt->Move(xChange, yChange);
	scoreText->Move(xChange, yChange);
	continueBtn->Resize(xChange, yChange);
	healthText->Move(xChange, yChange);
	stfVolUpBtn->Resize(xChange, yChange);
	resumeBtn->Resize(xChange, yChange);
	sfxDownBtn->Resize(xChange, yChange);

	for (int i = 0; i < (int)enemies.size(); i++)
		enemies[i]->Resize(xChange, yChange);
	
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
	resumeBtn->Reset();

	player->EmptyInput();

	if (gamePaused)
	{
		SDL_SetWindowResizable(window, SDL_TRUE);
		logObj->LogMethod("Game was unpaused");
	}
	else
	{
		SDL_SetWindowResizable(window, SDL_FALSE);
		logObj->LogMethod("Game was paused");
	}

	gamePaused = !gamePaused;
}


SDL_Renderer* JN_GameWorld::GetRenderer()
{
	return renderer;
}


std::vector<JN_GameObject*> JN_GameWorld::GetTiles()
{
	return allTiles;
}