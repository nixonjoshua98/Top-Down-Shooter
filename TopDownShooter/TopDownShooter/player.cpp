#include "stdafx.h"
#include "gameWorld.h"
#include "player.h"

#include <algorithm>
#include <iostream>

/* Header File Includes:
	"square.h"
	"SDL.h"
	"projectile.h"
	<map>
	<vector>
*/

void Player::Init(int x, int y, RGB col)
{
	Square::Init(x, y, col);

	rect.w = Player::PLAYER_WIDTH;
	rect.h = Player::PLAYER_HEIGHT;
}

void Player::Input(SDL_Event e)
{
	if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
	{
		// Valid keyboard control
		if (keyboardControls.find(e.key.keysym.scancode) != keyboardControls.end())
		{
			KeyboardInputHandler(e);
		}
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
	{
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
	// the key most likely has been releasedso i remove it from the vector
	else if (e.type == SDL_KEYUP && controlIter != keyboardPresses.end())
	{
		keyboardPresses.erase(controlIter);
	}
}

void Player::MouseInputHandler(SDL_Event e)
{
	switch (mouseControls[e.button.button])
	{
	case ControlEnum::SHOOT:
		triggerDown = (e.type == SDL_MOUSEBUTTONDOWN);
		break;
	}
}

void Player::Shoot()
{
	if (!readyToShoot || !triggerDown)
		return;

	int x, y;
	SDL_GetMouseState(&x, &y);

	Projectile p = Projectile();
	p.Init(rect.x + (rect.w / 2), rect.y + (rect.h / 2), x, y);
	projectiles.push_back(p);

	readyToShoot  = false;
	lastShootTime = SDL_GetTicks();
}

void Player::Update()
{
	int now = SDL_GetTicks();

	// Update projectiles - WTF BOII
	//for (Projectile p : projectiles)
		//p.Update();

	for (int i = 0; i < projectiles.size(); i++)
	{
		if (projectiles[i].OutOfBounds())
		{
			projectiles.erase(projectiles.begin() + i);
			continue;
		}

		projectiles[i].Update();
	}

	// Move timer
	if (now - lastMovementTime > MOVEMENT_DELAY)
	{
		lastMovementTime = now;
		Move();
	}

	// Shoot timer
	if ((now - lastShootTime > SHOOT_DELAY) && !readyToShoot)
	{
		readyToShoot = true;
	}

	Shoot();
}

void Player::Move()
{
	newRect.x = rect.x;
	newRect.y = rect.y;

	for (ControlEnum key : keyboardPresses)
	{
		switch (key)
		{
		case ControlEnum::LEFT:
			newRect.x -= Player::MOVE_AMOUNT;
			break;

		case ControlEnum::RIGHT:
			newRect.x += Player::MOVE_AMOUNT;
			break;

		case ControlEnum::UP:
			newRect.y -= Player::MOVE_AMOUNT;
			break;

		case ControlEnum::DOWN:
			newRect.y += Player::MOVE_AMOUNT;
			break;

		default:
			break;
		}
	}
}

void Player::LateUpdate()
{
	bool outsideMap = (newRect.x < 0 || newRect.y < 0) || 
		              (newRect.x + rect.w > GameWorld::WINDOW_WIDTH) || 
			          (newRect.y + rect.h > GameWorld::WINDOW_HEIGHT);

	if (outsideMap)
	{
		rect.x = (int)(fmin(GameWorld::WINDOW_WIDTH  -  PLAYER_WIDTH, fmax(0, rect.x)));
		rect.y = (int)(fmin(GameWorld::WINDOW_HEIGHT - PLAYER_HEIGHT, fmax(0, rect.y)));
	}
	else
	{
		rect.x = newRect.x;
		rect.y = newRect.y;
	}
}

void Player::Render(SDL_Renderer *renderer)
{
	// Render projectiles
	for (Projectile p : projectiles)
		p.Render(renderer);

	Square::Render(renderer);
}