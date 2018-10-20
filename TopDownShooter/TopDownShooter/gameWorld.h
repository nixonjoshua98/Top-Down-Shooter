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

	static const int WINDOW_WIDTH  = 640;
	static const int WINDOW_HEIGHT = 480;
	static const int LEVEL_HEIGHT  = 15;
	static const int LEVEL_WIDTH   = 20;

	bool Init();
	void Run();

private:
	// Constants
	static const int DELTA_TIME   = (1000 / 60);

	// Flags
	bool running = true;

	// Objects
	Player player = Player();
	Square worldArr[LEVEL_HEIGHT * LEVEL_WIDTH];

	// Pointers
	Square       *wallArrPtr   = worldArr;
	SDL_Window   *window       = NULL;
	SDL_Renderer *renderer     = NULL;

	// Methods
	void Update();
	void LateUpdate();
	void Render();
	void Input();

};

#endif // !GAME_WORLD_H
