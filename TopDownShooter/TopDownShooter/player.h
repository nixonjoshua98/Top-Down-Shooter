#ifndef PLAYER_H
#define PLAYER_H

#include "square.h"
#include <map>
#include <vector>
#include <set>
#include "projectileController.h"
#include "SDL.h"

class Player : public Square
{
public:
	int _ = 0;	// Used for testing & debugging

	void Init(int x, int y, RGB col);
	void Input(SDL_Event e);
	void Update();
	void LateUpdate(Square *worldArr);
	void Render(SDL_Renderer *renderer) override;

private:
	// Constants
	static const int MOVEMENT_SPEED = 10;
	static const int MOVEMENT_DELAY = 10;	// Otherwise it will depend on FPS
	static const int SHOOT_DELAY    = 100;
	static const int PLAYER_WIDTH   = 15;
	static const int PLAYER_HEIGHT  = 15;

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

	ProjectileController projectileController = ProjectileController(10);	// Creates the controller with a maximum of 10 projectiles on screen at once

	// Tracks key presses
	std::vector<ControlEnum> keyboardPresses;

	// Flags
	bool readyToShoot = false;
	bool triggerDown  = false;

	// Player buffs and debuffs
	std::map<SquareType, bool> buffs = {
		{SquareType::MOVEMENT_DEBUFF, false}
	};

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
	void ConfirmPlayerMovement();
	std::set<SquareType> GetColliders(Square *worldArr);
	void ColliderManager(Square *worldArr);
	void ResetBuffs();

};

#endif // !PLAYER_H
