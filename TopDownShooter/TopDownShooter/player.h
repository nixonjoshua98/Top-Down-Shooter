#ifndef PLAYER_H
#define PLAYER_H

#include "square.h"
#include <map>
#include <vector>
#include "projectile.h"
#include "SDL.h"

class Player : public Square
{
public:
	void Init(int x, int y, RGB col) override;
	void Input(SDL_Event e);
	void Update();
	void LateUpdate();
	void Render(SDL_Renderer *renderer) override;

private:
	// Constants
	static const int MOVE_AMOUNT    =   3;
	static const int MOVEMENT_DELAY =  10;	// Otherwise it will depend on FPS
	static const int SHOOT_DELAY    =  50;
	static const int PLAYER_WIDTH   =  10;
	static const int PLAYER_HEIGHT  =  10;

	// All possible actions
	enum class ControlEnum {LEFT, RIGHT, UP, DOWN, NONE, SHOOT};

	// Keyboard controls - map each key to different action
	std::map<int, ControlEnum> keyboardControls = {
		{  SDL_SCANCODE_A, ControlEnum::LEFT  },
		{  SDL_SCANCODE_D, ControlEnum::RIGHT },
		{  SDL_SCANCODE_W, ControlEnum::UP    },
		{  SDL_SCANCODE_S, ControlEnum::DOWN  },
	};

	// Mouse button control map
	std::map<int, ControlEnum> mouseControls = {
		{ SDL_BUTTON_LEFT, ControlEnum::SHOOT },
	};

	// Tracks key presses
	std::vector<ControlEnum> keyboardPresses;

	// Flags
	bool readyToShoot = false;
	bool triggerDown  = false;

	// Objects
	std::vector<Projectile> projectiles;

	// Rects
	SDL_Rect newRect = SDL_Rect();

	// Used in delays
	int lastMovementTime = 0;
	int lastShootTime    = 0;

	// Methods
	void Move();
	void Shoot();
	void KeyboardInputHandler(SDL_Event e);
	void MouseInputHandler(SDL_Event e);

};

#endif // !PLAYER_H
