#ifndef JN_GAME_WORLD_H
#define JN_GAME_WORLD_H

#include <SDL.h>
#include <vector>

#include "JN_Player.h"
#include "JN_PerformanceTimer.h"
#include "JN_Gameobject.h"
#include "JN_Logging.h"
#include "JN_GameplayTimer.h"
#include "JN_WindowData.h"
#include "JN_Text.h"


class JN_GameWorld
{
public:
	///<summary>The default constructor which takes zero parameters</summary>
	JN_GameWorld();


	///<summary>The deconstructor which deallocates objects from memory</summary>
	~JN_GameWorld();

	static const int FPS = 60;											// Frames per second aim
	static const int MIN_WINDOW_WIDTH  = 960;							// Starting width of the entire window in px
	static const int MIN_WINDOW_HEIGHT = 720;							// Min height of the entire window in px
	static const int LEVEL_HEIGHT = 20;									// Cells high the game world is	
	static const int LEVEL_WIDTH = 20;									// Width of the world in cells
	static const int WORLD_WIDTH = MIN_WINDOW_WIDTH;					// Width of the world in px
	static const int WORLD_HEIGHT = MIN_WINDOW_HEIGHT;					// Height of the window
	static const int LEVEL_SIZE = LEVEL_WIDTH * LEVEL_HEIGHT;			// Total cells	
	static const int CELL_HEIGHT = WORLD_HEIGHT / LEVEL_HEIGHT;			// Height of each cell
	static const int CELL_WIDTH = WORLD_WIDTH / LEVEL_WIDTH;			// Width of each cell

	static const int FULL_SCREEN_KEY = SDL_SCANCODE_1;					// ...
	static const int LOG_TOGGLE_KEY = SDL_SCANCODE_2;					// ...
	static const int PAUSE_GAME_KEY = SDL_SCANCODE_ESCAPE;				// ...


	// Used or resizing objects, used as a pointer so the window height and width is always accurate
	JN_WindowData *windowData = NULL;


	///<summary>Initialises the window, renderer and starts building the world</summary>
	bool Init();


	///<summary>The game loop which will be run thoughtout the game sequence</summary>
	void Run();

private:
	bool running    = true;		// Boolean flag which keeps the game loop going if the game is active
	bool fullscreen = false;	// Flag to check if game is full screen or not
	bool gamePaused = false;	// ...
	bool timerComplete = false;	// ...

	JN_PerformanceTimer timer;			// Timer class which stores FPS
	JN_Player player = JN_Player();		// Player object, the user will control this objects
	JN_Text *timerText = NULL;			// 60 seconds timer text
	JN_GameplayTimer gameplayTimer;		// gameplay timer (60secs)

	std::vector<JN_Gameobject*> emptyTiles = {};		// Vector which stores all of the tiles which will not need to be taken into accord during collision detection
	std::vector<JN_Gameobject*> collisionTiles = {};	// Tiles which the player (or other objects) can collide with
	std::vector<JN_Gameobject*> allTiles = {};			// Vector of pointers to ALL world tiles
	JN_Logging *logObj = NULL;							// Object used to log to console and file
	SDL_Window *window = NULL;							// Pointer to the game window
	SDL_Renderer *renderer = NULL;						// Pointer to the renderer which the entire game will use

	char charWorldArray[LEVEL_SIZE];	// Used when loading in the world text file
	float gameDuration = 0.0f;


	///<summary>Calls .Update() on all needed objects</summary>
	void Update();


	///<summary>Calls .LateUpdate() on all needed objects</summary>
	void LateUpdate();


	///<summary>Calls .Render() on all needed objects</summary>
	void Render();


	///<summary>Calls .Input() on all needed objects</summary>
	void Input();


	///<summary>Loads the world text file into an array</summary>
	void LoadWorldFile();


	///<summary>Creates the world based on the loaded in text file </summary>
	void BuildWorld();


	///<summary>Method which is called upon the player resizing the window</summary>
	void ResizeWorld();


	///<summary>Toggles the windows view display from fullscreen to shown</summary>
	void ToggleFullScreen();


	///<summary>Pause the game</summary>
	void TogglePauseGame();
};

#endif // !JN_GAME_WORLD_H
