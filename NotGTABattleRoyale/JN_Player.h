#ifndef JN_PLAYER_H
#define JN_PLAYER_H

#include "JN_Sprite.h"
#include "JN_ProjectileController.h"
#include "SDL.h"

#include <map>
#include <vector>
#include <set>

class JN_Player : public JN_Sprite
{
public:
	~JN_Player();

	int _ = 0;	// Used for testing & debugging

	void Init(SDL_Renderer *renderer);
	void Input(SDL_Event e);
	void Update();
	void LateUpdate(std::vector<JN_Sprite*> tiles);
	void Render(SDL_Renderer *renderer) override;

private:
	// Constants
	static const int MOVEMENT_SPEED = 5;
	static const int MOVEMENT_DELAY = 10;	// Otherwise it will depend on FPS
	static const int SHOOT_DELAY    = 250;
	static const int PLAYER_WIDTH   = 20;
	static const int PLAYER_HEIGHT  = 20;

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

	JN_ProjectileController projectileController = JN_ProjectileController(10);	// Creates the controller with a maximum of 10 projectiles on screen at once

	// Tracks key presses
	std::vector<ControlEnum> keyboardPresses;

	// Flags
	bool readyToShoot = false;
	bool triggerDown  = false;

	// Player buffs and debuffs
	std::map<SpriteType, bool> buffs = {
		{ SpriteType::MOVEMENT_DEBUFF, false}
	};

	// Rects
	SDL_Rect newRect = SDL_Rect();

	// Used in delays
	float lastMovementTime = 0;
	float lastShootTime    = 0;

	// Methods
	void Move();
	void Shoot();
	void KeyboardInputHandler(SDL_Event e);
	void MouseInputHandler(SDL_Event e);
	void ConfirmPlayerMovement();
	std::set<SpriteType> GetColliders(std::vector<JN_Sprite*> tiles);
	void ColliderManager(std::vector<JN_Sprite*> tiles);
	void ResetBuffs();
	std::string Keypress2Name(int code);

};

#endif // !PLAYER_H
