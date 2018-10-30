
#include "stdafx.h"
#include "gameWorld.h"
#include "player.h"

#include <algorithm>
#include <iostream>

/* Header File Includes:
	"square.h"
	"SDL.h"
	"projectileController.h"
	<map>
*/

void Player::Init(int x, int y, RGB col)
{
	// Calls the base class constructor
	Square::Init(SquareType::PLAYER, x, y, col);

	rect.w	  = Player::PLAYER_WIDTH;
	rect.h	  = Player::PLAYER_HEIGHT;
	newRect.w = rect.w;
	newRect.h = rect.h;
}

void Player::Input(SDL_Event e)
{
	// Keyboard key has been released or presses
	if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
	{
		// Valid keyboard control
		if (keyboardControls.find(e.key.keysym.scancode) != keyboardControls.end())
		{
			KeyboardInputHandler(e);
		}
	}
	// Mouse button has been clicked or released
	else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
	{
		// Mouse button is a valid control
		if (mouseControls.find(e.button.button) != mouseControls.end())
		{
			MouseInputHandler(e);
		}
	}
}

void Player::KeyboardInputHandler(SDL_Event e)
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

void Player::MouseInputHandler(SDL_Event e)
{
	// Checks if the mouse button is mapped to a control
	switch (mouseControls[e.button.button])
	{
	case ControlEnum::SHOOT:
		triggerDown = (e.type == SDL_MOUSEBUTTONDOWN);
		break;
	}
}

void Player::Shoot()
{
	// If the shoot cooldown has passed then this will be set to true
	readyToShoot = (SDL_GetTicks() - lastShootTime > SHOOT_DELAY);

	if (!readyToShoot || !triggerDown)
		return;

	int x, y;	// Get the mouse position
	SDL_GetMouseState(&x, &y);

	// Setup the target rect
	SDL_Rect target = SDL_Rect();
	target.x = x;
	target.y = y;

	if (projectileController.Shoot(rect, target))
	{
		readyToShoot = false;
		lastShootTime = SDL_GetTicks();
	}
}

void Player::Update()
{
	projectileController.Update();
	Move();
	Shoot();
}

void Player::Move()
{
	int now = SDL_GetTicks();

	if (!now - lastMovementTime > MOVEMENT_DELAY) { return; }

	lastMovementTime = now;

	newRect.x = rect.x;
	newRect.y = rect.y;

	float movementMultiplier = (buffs[SquareType::MOVEMENT_DEBUFF] ? 0.5f : 1.0f);

	for (ControlEnum key : keyboardPresses)
	{
		switch (key)
		{
		case ControlEnum::LEFT:
			newRect.x -= (int)(Player::MOVEMENT_SPEED * movementMultiplier);
			break;

		case ControlEnum::RIGHT:
			newRect.x += (int)(Player::MOVEMENT_SPEED * movementMultiplier);
			break;

		case ControlEnum::UP:
			newRect.y -= (int)(Player::MOVEMENT_SPEED * movementMultiplier);
			break;

		case ControlEnum::DOWN:
			newRect.y += (int)(Player::MOVEMENT_SPEED * movementMultiplier);
			break;
		}
	}
}

void Player::LateUpdate(Square *worldArr)
{
	ConfirmPlayerMovement();
	ColliderManager(worldArr);
	projectileController.LateUpdate();
}

void Player::ColliderManager(Square *worldArr)
{
	// Transform the set to a vector which is easier to iterate over
	std::set<SquareType> colliders = GetColliders(worldArr);
	std::vector<SquareType> collidersV = {};
	collidersV.assign(colliders.begin(), colliders.end());
	// TODO: The above code should be changed but it works well and i dont want to have to debug the new errors

	ResetBuffs();

	// Current colliders
	for (int i = 0; i < collidersV.size(); i++)
	{
		switch (collidersV[i])
		{
		case SquareType::MOVEMENT_DEBUFF:
			buffs[SquareType::MOVEMENT_DEBUFF] = true;
			break;
		}
	}
}

void Player::ResetBuffs()
{
	// Sets all buffs to false;
	for (const auto & kv : buffs)
	{
		buffs[kv.first] = false;
	}
}

void Player::ConfirmPlayerMovement()
{
	// Makes sure that the player is always within the screen boundaries
	newRect.x = (int)(fmin(GameWorld::WINDOW_WIDTH  - PLAYER_WIDTH, fmax(0, newRect.x)));
	newRect.y = (int)(fmin(GameWorld::WINDOW_HEIGHT - PLAYER_HEIGHT, fmax(0, newRect.y)));

	rect.x = newRect.x;
	rect.y = newRect.y;
}

// TIL - Returning a set pointer is evil and i should never attempt it again
std::set<Square::SquareType> Player::GetColliders(Square *worldArr)
{
	std::set<SquareType> colliders = {};	// We only care about each type, not quantity

	for (int i = 0; i < GameWorld::LEVEL_SIZE; i++)
	{
		// Spent about an hour trying to fix this, I had + 1 instead of + i
		if (Collide(rect, (worldArr + i)->rect))
			colliders.insert((worldArr + i)->squareType);
	}
	return colliders;
}

void Player::Render(SDL_Renderer *renderer)
{
	projectileController.Render(renderer);
	Square::Render(renderer);
}