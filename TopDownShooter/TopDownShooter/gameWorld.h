#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "SDL.h"
#include "square.h"
#include "player.h"

class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	// Constants
	static const int WINDOW_WIDTH  = 640;
	static const int WINDOW_HEIGHT = 480;
	static const int LEVEL_HEIGHT  = 15;	
	static const int LEVEL_WIDTH   = 20;
	static const int LEVEL_SIZE    = LEVEL_WIDTH * LEVEL_HEIGHT;	// Total cells	
	static const int CELL_HEIGHT   = WINDOW_HEIGHT / LEVEL_HEIGHT;
	static const int CELL_WIDTH    = WINDOW_WIDTH / LEVEL_WIDTH;
	static const int DELTA_TIME    = (1000 / 15);	// FPS

	///<summary>Initializes SDL and creates the window & renderer</summary>
	bool Init();

	///<summary>The game loop which runs the game<summary>
	void Run();

private:
	// Flags
	bool running = true;

	// Objects
	Player player = Player();
	Square worldArr    [LEVEL_SIZE];
	char charWorldArray[LEVEL_SIZE];	// Used when loading in World.txt

	// Pointers
	Square       *worldArrArrPtr = worldArr;
	SDL_Window   *window         = NULL;
	SDL_Renderer *renderer       = NULL;

	///<summary>Calls the update method on the active objects</summary>
	void Update();

	///<summary>Calls the late update method on the active objects</summary>
	void LateUpdate();

	///<summary>Calls the render method on the active objects</summary>
	void Render();

	///<summary>Calls the input method on the moveable objects</summary>
	void Input();

	///<summary>Reads the world text file</summary>
	void LoadWorldFile();

	///<summary>Builds the world based on the char world array</sumamry>
	void BuildWorld();
};

#endif // !GAME_WORLD_H
