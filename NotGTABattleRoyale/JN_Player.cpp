
#include "stdafx.h"

#include "JN_GameWorld.h"
#include "JN_Player.h"
#include "JN_Logging.h"

#include <algorithm>
#include <iostream>
#include <math.h>

JN_Player::~JN_Player()
{
	JN_Log("Player", "Deconstructor called", false);
}

void JN_Player::Init(SDL_Renderer *renderer)
{
	// Calls the base class constructor
	JN_Sprite::Init(SpriteType::PLAYER, renderer, rect, 3);

	rect.w = JN_Player::PLAYER_WIDTH;
	rect.h = JN_Player::PLAYER_HEIGHT;
	rect.x = 0;
	rect.y = JN_GameWorld::BANNER_HEIGHT;

	newRect.w = rect.w;
	newRect.h = rect.h;

	projectileController.CreateInitialProjectiles(renderer);
}

void JN_Player::Input(SDL_Event e)
{
	// Keyboard key has been released or presses
	if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
	{
		// Valid keyboard control
		if (keyboardControls.find(e.key.keysym.scancode) != keyboardControls.end())
			KeyboardInputHandler(e);
	}
	// Mouse button has been clicked or released
	else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
	{
		// Mouse button is a valid control
		if (mouseControls.find(e.button.button) != mouseControls.end())
			MouseInputHandler(e);
	}
}

void JN_Player::KeyboardInputHandler(SDL_Event e)
{
	// Finds the value in the key presses vector, if value is .end() then the value was not found
	std::vector<ControlEnum>::iterator controlIter;
	controlIter = std::find(keyboardPresses.begin(), keyboardPresses.end(), keyboardControls[e.key.keysym.scancode]);

	// A new valid control key has been pressed
	if (e.type == SDL_KEYDOWN && controlIter == keyboardPresses.end())
	{
		keyboardPresses.push_back(keyboardControls[e.key.keysym.scancode]);
	}

	// A valid input has been detected which has already been added to the vector
	// the key most likely has been released so i remove it from the vector
	else if (e.type == SDL_KEYUP && controlIter != keyboardPresses.end())
	{
		keyboardPresses.erase(controlIter);
	}
}

void JN_Player::MouseInputHandler(SDL_Event e)
{
	// Checks if the mouse button is mapped to a control
	switch (mouseControls[e.button.button])
	{
	case ControlEnum::SHOOT:
		triggerDown = (e.type == SDL_MOUSEBUTTONDOWN);
		break;
	}
}

void JN_Player::Shoot()
{
	float now = (float)SDL_GetTicks();

	// If the shoot cooldown has passed then this will be set to true
	readyToShoot = (now - lastShootTime > SHOOT_DELAY);

	if (!readyToShoot || !triggerDown)
		return;

	int x, y;	// Get the mouse position
	SDL_GetMouseState(&x, &y);

	// Setup the target rect (Take the player coords into account)
	SDL_Rect target = SDL_Rect();
	target.x = (x - 2) - rect.x;
	target.y = (y - 2) - rect.y;

	SDL_Rect sourceRect = SDL_Rect();
	sourceRect.x = rect.x + (rect.w / 2);
	sourceRect.y = rect.y + (rect.h / 2);

	if (projectileController.Shoot(sourceRect, target))
	{
		readyToShoot  = false;
		lastShootTime = now;
	}
}

void JN_Player::Update()
{
	projectileController.Update();
	Move();
	Shoot();
}

void JN_Player::Move()
{
	float now = (float)SDL_GetTicks();

	if (!now - lastMovementTime > MOVEMENT_DELAY) { return; }

	lastMovementTime = now;

	newRect.x = rect.x;
	newRect.y = rect.y;

	float movementMultiplier = (buffs[SpriteType::MOVEMENT_DEBUFF] ? 0.25f : 1.0f);


	for (ControlEnum key : keyboardPresses)
	{
		switch (key)
		{
		case ControlEnum::LEFT:
			newRect.x -= (int)(MOVEMENT_SPEED * movementMultiplier);
			break;

		case ControlEnum::RIGHT:
			newRect.x += (int)(MOVEMENT_SPEED * movementMultiplier);
			break;

		case ControlEnum::UP:
			newRect.y -= (int)(MOVEMENT_SPEED * movementMultiplier);
			break;

		case ControlEnum::DOWN:
			newRect.y += (int)(MOVEMENT_SPEED * movementMultiplier);
			break;
		}
	}

	if (now - lastSpriteChange > spriteChangeDelay)
	{
		if (keyboardPresses.size() > 0)
		{
			lastSpriteChange = now;
			spriteIndex = spriteIndex == 1 ? 2 : 1;
		}

		else if (keyboardPresses.size() == 0)
			spriteIndex = 0;
	}

	int x, y;
	SDL_GetMouseState(&x, &y);
	rotationAngle = atan2((y - 2) - newRect.y, (x - 2)- newRect.x) * 180.0f / 3.14159;
}

void JN_Player::LateUpdate(std::vector<JN_Sprite*> tiles)
{
	ConfirmPlayerMovement();
	ColliderManager(tiles);
	projectileController.LateUpdate();
}

void JN_Player::ColliderManager(std::vector<JN_Sprite*> tiles)
{
	// Transform the set to a vector which is easier to iterate over
	std::set<SpriteType> colliders = GetColliders(tiles);
	std::vector<SpriteType> collidersV = {};
	collidersV.assign(colliders.begin(), colliders.end());
	// TODO: The above code should be changed but it works well and i dont want to have to debug the new errors

	ResetBuffs();

	// Current colliders
	for (unsigned int i = 0; i < collidersV.size(); i++)
	{
		switch (collidersV[i])
		{
		case SpriteType::MOVEMENT_DEBUFF:
			buffs[SpriteType::MOVEMENT_DEBUFF] = true;
			break;
		}
	}
}

void JN_Player::ResetBuffs()
{
	JN_Log("Player", "ResetBuffs", false);

	// Sets all buffs to false;
	for (const auto & kv : buffs)
		buffs[kv.first] = false;
}

void JN_Player::ConfirmPlayerMovement()
{
	JN_Log("Player", "ConfirmPlayerMovement", false);

	// Makes sure that the player is always within the screen boundaries
	newRect.x = (int)(fmin(JN_GameWorld::WORLD_WIDTH  - PLAYER_WIDTH, fmax(0, newRect.x)));
	newRect.y = (int)(fmin(JN_GameWorld::WORLD_HEIGHT + JN_GameWorld::BANNER_HEIGHT - PLAYER_HEIGHT, fmax(JN_GameWorld::BANNER_HEIGHT, newRect.y)));

	rect.x = newRect.x;
	rect.y = newRect.y;
}

// TIL - Returning a set pointer is evil and i should never attempt it again
std::set<JN_Player::SpriteType> JN_Player::GetColliders(std::vector<JN_Sprite*> tiles)
{
	JN_Log("Player", "GetColliders", false);

	std::set<SpriteType> colliders = {};	// We only care about each type, not quantity

	for (int i = 0; i < (int)tiles.size(); i++)
	{
		// Spent about an hour trying to fix this, I had + 1 instead of + i
		if (Collide(rect, tiles[i]->rect))
			colliders.insert(tiles[i]->type);
	}

	JN_Log("Player", "GetColliders [END]", false);
	return colliders;
}

void JN_Player::Render(SDL_Renderer *renderer)
{
	JN_Sprite::Render(renderer);
	projectileController.Render(renderer);
}

// Used for logging inputs
std::string JN_Player::Keypress2Name(int code)
{
	switch (keyboardControls[code])
	{
	case ControlEnum::DOWN:
		return "down";

	case ControlEnum::UP:
		return "up";

	case ControlEnum::LEFT:
		return "left";

	case ControlEnum::RIGHT:
		return "right";

	default:
		return "keycode not found";
	}
}