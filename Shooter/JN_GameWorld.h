#ifndef JN_GAME_WORLD_H
#define JN_GAME_WORLD_H

#include <SDL.h>
#include <vector>
#include <SDL_mixer.h>

#include "JN_PerformanceTimer.h"
#include "JN_GameplayTimer.h"
#include "JN_GameObject.h"
#include "JN_WindowData.h"
#include "JN_Logging.h"
#include "JN_Button.h"
#include "JN_Player.h"
#include "JN_Enemy.h"
#include "JN_Text.h"


class JN_GameWorld
{
public:

	int _ = 0;


	///<summary>The default constructor which takes zero parameters</summary>
	JN_GameWorld();


	///<summary>The deconstructor which deallocates objects from memory</summary>
	~JN_GameWorld();

	static const int FPS = 40;											// Frames per second aim
	static const int MIN_WINDOW_WIDTH  = 960;							// Starting width of the entire window in px
	static const int MIN_WINDOW_HEIGHT = 720;							// Min height of the entire window in px
	static const int LEVEL_HEIGHT = 20;									// Cells high the game world is	
	static const int LEVEL_WIDTH = 20;									// Width of the world in cells
	static const int WORLD_WIDTH = MIN_WINDOW_WIDTH;					// Width of the world in px
	static const int WORLD_HEIGHT = MIN_WINDOW_HEIGHT;					// Height of the window
	static const int LEVEL_SIZE = LEVEL_WIDTH * LEVEL_HEIGHT;			// Total cells	
	static const int CELL_HEIGHT = WORLD_HEIGHT / LEVEL_HEIGHT;			// Height of each cell
	static const int CELL_WIDTH = WORLD_WIDTH / LEVEL_WIDTH;			// Width of each cell

	static const int MAXIMUM_ENEMIES = 32;								// ...

	static const int FULL_SCREEN_KEY = SDL_SCANCODE_F11;				// ...
	static const int LOG_TOGGLE_KEY = SDL_SCANCODE_F1;					// ...
	static const int PAUSE_GAME_KEY = SDL_SCANCODE_ESCAPE;				// ...

	static bool isSfxMute;

	static Mix_Chunk* buttonClick;

	bool isQuit = false;

	static Mix_Music* bgm;
	
	Mix_Chunk* oof = NULL;
	Mix_Chunk* coins = NULL;

	int bgmVolume = 10;
	int sfxVolume = 40;

	static const int MAX_BGM_VOLUME = 10;
	static const int MAX_SFX_VOLUME = 40;

	static int vol;

	// Used or resizing objects, used as a pointer so the window height and width is always accurate
	JN_WindowData *windowData = NULL;


	///<summary>Initialises the window, renderer and starts building the world</summary>
	bool Init();


	///<summary>The game loop which will be run thoughtout the game sequence</summary>
	void Run();


	SDL_Renderer* GetRenderer();

	std::vector<JN_GameObject*> GetTiles();

	int GetBGMVolume()
	{
		return bgmVolume;
	}

	int GetSFXVolume()
	{
		return sfxVolume;
	}

	void SetBGMVolume(int i)
	{
		bgmVolume = fminf(100, i);
		bgmVolume = fmax(0, bgmVolume);
	}

	void SetSFXVolume(int i)
	{
		sfxVolume = fminf(100, i);
		sfxVolume = fmax(0, sfxVolume);
	}

	JN_Logging* GetLog()
	{
		return logObj;
	}

private:
	bool running    = true;		// Boolean flag which keeps the game loop going if the game is active
	bool fullscreen = false;	// Flag to check if game is full screen or not
	bool gamePaused = false;	// ...
	bool timerComplete = false;	// ...
	bool gameStarted = true;

	JN_Button* continueBtn = NULL;



	JN_PerformanceTimer performanceTimer;	// Timer class which stores FPS
	JN_Player *player = NULL;				// Player object, the user will control this objects
	JN_Text *timerText = NULL;				// 60 seconds timer text
	JN_Text *scoreText = NULL;
	JN_Text *healthText = NULL;

	JN_Button *sfxDownBtn = NULL;
	JN_Text* sfxVolTxt = NULL;
	JN_Button* stfVolUpBtn = NULL;
	JN_Text* bgmVolTxt = NULL;
	JN_Button* bgmVolUpBtn = NULL;
	JN_Button* bgmVolDownBtn = NULL;

	JN_Button *resumeBtn = NULL;
	JN_GameplayTimer gameplayTimer;			// gameplay timer (60secs)

	std::vector<JN_GameObject*> emptyTiles = {};		// Vector which stores all of the tiles which will not need to be taken into accord during collision detection
	std::vector<JN_GameObject*> collisionTiles = {};	// Tiles which the player (or other objects) can collide with
	std::vector<JN_GameObject*> allTiles = {};			// Vector of pointers to ALL world tiles
	std::vector<JN_Enemy*> enemies = {};				// Vector to store all of the enemies in the world
	JN_Logging *logObj = NULL;							// Object used to log to console and file
	SDL_Window *window = NULL;							// Pointer to the game window
	SDL_Renderer *renderer = NULL;						// Pointer to the renderer which the entire game will use

	int enemiesSpawned = 0;

	char charWorldArr[LEVEL_SIZE];	// Used when loading in the world text file

	float gameDuration = 0.0f;
	float lastEnemySpawn = 0.0f;

	///<summary>Setup the application<summary>
	void Setup();


	//void PostGameMenu();


	///<summary>Calls .Update() on all needed objects</summary>
	void Update();


	///<summary>Calls .LateUpdate() on all needed objects</summary>
	void LateUpdate();


	///<summary>Calls .Render() on all needed objects</summary>
	void Render();


	///<summary>Calls .Input() on all needed objects</summary>
	void Input();

	void CreateRandomWorldMap();


	///<summary>Creates the world based on the loaded in text file </summary>
	void BuildWorld();


	///<summary>Method which is called upon the player resizing the window</summary>
	void ResizeWorld();


	///<summary>Toggles the windows view display from fullscreen to shown</summary>
	void ToggleFullScreen();


	///<summary>Pause the game</summary>
	void TogglePauseGame();


	///<summary>Spawn an enemy if allowed</summary>
	void SpawnEnemy();
};

#endif // !JN_GAME_WORLD_H
