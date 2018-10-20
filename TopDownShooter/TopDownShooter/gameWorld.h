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
	static const int LEVEL_SIZE    = LEVEL_WIDTH * LEVEL_HEIGHT;
	static const int CELL_HEIGHT   = WINDOW_HEIGHT / LEVEL_HEIGHT;
	static const int CELL_WIDTH    = WINDOW_WIDTH / LEVEL_WIDTH;
	static const int DELTA_TIME    = (1000 / 60);	// FPS

	bool Init();
	void Run();

private:
	// Flags
	bool running = true;

	// Objects
	Player player = Player();
	Square worldArr    [LEVEL_SIZE];
	char charWorldArray[LEVEL_SIZE];

	// Pointers
	Square       *worldArrArrPtr = worldArr;
	SDL_Window   *window         = NULL;
	SDL_Renderer *renderer       = NULL;

	// Methods
	void Update();
	void LateUpdate();
	void Render();
	void Input();

	void LoadWorldFile();
	void BuildWorld();
};

#endif // !GAME_WORLD_H
