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
	JN_GameWorld();
	~JN_GameWorld();

	// Constants
	static const int WINDOW_WIDTH  = 640;
	static const int WINDOW_HEIGHT = 480;
	static const int WORLD_WIDTH   = WINDOW_WIDTH;
	static const int WORLD_HEIGHT  = 450;
	static const int BANNER_HEIGHT = WINDOW_HEIGHT - WORLD_HEIGHT;

	static const int LEVEL_HEIGHT  = 15;	
	static const int LEVEL_WIDTH   = 20;
	static const int LEVEL_SIZE    = LEVEL_WIDTH * LEVEL_HEIGHT;	// Total cells	
	static const int CELL_HEIGHT   = WINDOW_HEIGHT / LEVEL_HEIGHT;
	static const int CELL_WIDTH    = WINDOW_WIDTH / LEVEL_WIDTH;
	static const int FPS	       = 20;

	bool Init();	// Initializes SDL and creates the window & renderer
	void Run();		// The game loop which runs the game

private:
	bool running = true;

	JN_Timer timer   = JN_Timer(FPS);
	JN_Player player = JN_Player();

	std::vector<JN_Sprite*> emptyTiles     = {};
	std::vector<JN_Sprite*> collisionTiles = {};

	char charWorldArray[LEVEL_SIZE];	// Used when loading in World.txt

	// Pointers
	SDL_Window *window     = NULL;
	SDL_Renderer *renderer = NULL;

	void Update();
	void LateUpdate();
	void Render();
	void Input();
	void LoadWorldFile();
	void BuildWorld();
};

#endif // !JN_GAME_WORLD_H
