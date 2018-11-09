#ifndef JN_GAME_WORLD_H
#define JN_GAME_WORLD_H

#include "SDL.h"

#include "JN_Player.h"
#include "JN_Timer.h"
#include "JN_Sprite.h"

#include <vector>

class JN_GameWorld
{
public:
	///<summary>The default constructor which takes zero parameters</summary>
	JN_GameWorld();


	///<summary>The deconstructor which deallocates objects from memory</summary>
	~JN_GameWorld();

	static const int WINDOW_WIDTH  = 640;								// Width of the entire window in px
	static const int WINDOW_HEIGHT = 480;								// Height of the entire window in px
	static const int BANNER_HEIGHT = 30;								// Height of the banner at the top of the screen
	static const int FPS           = 20;								// Frames per second aim
	static const int LEVEL_HEIGHT  = 15;								// Cells high the game world is	
	static const int LEVEL_WIDTH   = 20;								// Width of the world in cells
	static const int WORLD_WIDTH   = WINDOW_WIDTH;						// Width of the world in px
	static const int WORLD_HEIGHT  = WINDOW_HEIGHT -  BANNER_HEIGHT;	// Height of the window minus the height of the banner
	static const int LEVEL_SIZE    = LEVEL_WIDTH * LEVEL_HEIGHT;		// Total cells	
	static const int CELL_HEIGHT   = WINDOW_HEIGHT / LEVEL_HEIGHT;		// Height of each cell
	static const int CELL_WIDTH    = WINDOW_WIDTH / LEVEL_WIDTH;		// Width of each cell

	///<summary>Initialises the window, renderer and creates some objects such as the player</summary>
	bool Init();

	///<summary>The game loop which will be run thoughtout the game sequence</summary>
	void Run();

private:
	bool running     = true;			// Boolean flag which keeps the game loop going if the game is active
	JN_Timer timer   = JN_Timer(FPS);	// Timer class which stores FPS, playtime etc.
	JN_Player player = JN_Player();		// Player object, the user will control this objects

	std::vector<JN_Sprite*> emptyTiles     = {};	// Vector which stores all of the tiles which will not need to be taken into accord during collision detection
	std::vector<JN_Sprite*> collisionTiles = {};	// Tiles which the player (or other objects) can collide with

	char charWorldArray[LEVEL_SIZE];	// Used when loading in the world text file

	SDL_Window *window     = NULL;	// Pointer to the game window
	SDL_Renderer *renderer = NULL;	// Pointer to the renderer which the entire game will use

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
};

#endif // !JN_GAME_WORLD_H
