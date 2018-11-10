#include "stdafx.h"

#include "JN_GameWorld.h"
#include "JN_Player.h"
#include "JN_Logging.h"

#include <algorithm>
#include <iostream>
#include <math.h>


JN_Player::JN_Player()
{

}


JN_Player::~JN_Player()
{
	// Deconstructor
}


void JN_Player::Init(SDL_Renderer *renderer)
{
	// Calls the base class constructor
	JN_Sprite::Init(SpriteType::PLAYER, renderer, rect, 3);

	rect.w = JN_Player::PLAYER_WIDTH;
	rect.h = JN_Player::PLAYER_HEIGHT;
	rect.x = rect.w;
	rect.y = JN_GameWorld::BANNER_HEIGHT + rect.h;

	newRect.w = rect.w;
	newRect.h = rect.h;

	projectileController.CreateInitialProjectiles(renderer);
}


void JN_Player::Input(SDL_Event e)
{
	// Keyboard key has been released or presses
	if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN && controls.ValidControl(JN_PlayerControls::InputDevice::KEYBOARD, e.key.keysym.scancode))
		KeyboardInputHandler(e);

	// Mouse button has been clicked or released
	else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP && controls.ValidControl(JN_PlayerControls::InputDevice::MOUSE, e.button.button))
		MouseInputHandler(e);
}


void JN_Player::KeyboardInputHandler(SDL_Event e)
{
	bool keyPressedDown = controls.IsKeyDown(JN_PlayerControls::InputDevice::KEYBOARD, e.key.keysym.scancode);

	// New key has been pressed
	if (e.type == SDL_KEYDOWN && !keyPressedDown)
		controls.AddKeyPress(JN_PlayerControls::InputDevice::KEYBOARD, e.key.keysym.scancode);

	// Old keypress has been lifted
	else if (e.type == SDL_KEYUP && keyPressedDown)
		controls.RemoveKeyPress(JN_PlayerControls::InputDevice::KEYBOARD, e.key.keysym.scancode);
}


void JN_Player::MouseInputHandler(SDL_Event e)
{
	bool keyPressedDown = controls.IsKeyDown(JN_PlayerControls::InputDevice::MOUSE, e.button.button);

	// New mouse click
	if (e.type == SDL_MOUSEBUTTONDOWN && !keyPressedDown)
		controls.AddKeyPress(JN_PlayerControls::InputDevice::MOUSE, e.button.button);
	
	// Old mouse click has been lifted
	else if (e.type == SDL_MOUSEBUTTONUP && keyPressedDown)
		controls.RemoveKeyPress(JN_PlayerControls::InputDevice::MOUSE, e.button.button);
}


void JN_Player::Shoot()
{
	float now = (float)SDL_GetTicks();

	bool readyToShoot = (now - lastShootTime > SHOOT_DELAY);
	bool triggerDown  = controls.IsKeyDown(JN_PlayerControls::InputDevice::MOUSE, JN_PlayerControls::ControlAction::SHOOT);

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
		lastShootTime = now;
}


void JN_Player::Update()
{
	projectileController.Update();
	Move();
	RotatePlayer();
	Shoot();
	AnimationUpdate();
}


void JN_Player::Move()
{
	float now = (float)SDL_GetTicks();

	if (now - lastMovementTime < MOVEMENT_DELAY)
		return;

	lastMovementTime = now;

	newRect.x = rect.x;
	newRect.y = rect.y;

	float movementMultiplier = statusEffects[SpriteType::MOVEMENT_DEBUFF] ?  (1.0f / JN_Sprite::MOVEMENT_DEBUFF_AMOUNT) : 1.0f;

	for (JN_PlayerControls::ControlAction key : controls.GetKeyboardPresses())
	{
		switch (key)
		{
		case JN_PlayerControls::ControlAction::LEFT:
			newRect.x -= (int)(MOVEMENT_SPEED * movementMultiplier);
			break;

		case JN_PlayerControls::ControlAction::RIGHT:
			newRect.x += (int)(MOVEMENT_SPEED * movementMultiplier);
			break;

		case JN_PlayerControls::ControlAction::UP:
			newRect.y -= (int)(MOVEMENT_SPEED * movementMultiplier);
			break;

		case JN_PlayerControls::ControlAction::DOWN:
			newRect.y += (int)(MOVEMENT_SPEED * movementMultiplier);
			break;
		}
	}
}


void JN_Player::RotatePlayer()
{
	int x, y;
	SDL_GetMouseState(&x, &y);
	rotationAngle = atan2((y - 2) - newRect.y, (x - 2) - newRect.x) * 180.0f / 3.14159;
}


void JN_Player::LateUpdate(std::vector<JN_Sprite*> tiles)
{
	ConfirmPlayerMovement();
	ColliderManager(tiles);
	projectileController.LateUpdate();
}


void JN_Player::ColliderManager(std::vector<JN_Sprite*> tiles)
{
	std::set<SpriteType> colliders = GetColliders(tiles);

	ResetBuffs();

	// Current colliders
	for (auto c : colliders)
	{
		switch (c)
		{
		case SpriteType::MOVEMENT_DEBUFF:
			statusEffects[SpriteType::MOVEMENT_DEBUFF] = true;
			break;

		case SpriteType::FIRE_DAMAGE:
			health.TakeDamage(FIRE_DAMAGE);
		}
	}
}


void JN_Player::ResetBuffs()
{
	for (const auto & kv : statusEffects)
		statusEffects[kv.first] = false;
}


void JN_Player::ConfirmPlayerMovement()
{
	// Makes sure that the player is always within the screen boundaries
	newRect.x = (int)(fmin(JN_GameWorld::MIN_WINDOW_WIDTH  - PLAYER_WIDTH, fmax(0, newRect.x)));
	newRect.y = (int)(fmin(JN_GameWorld::MIN_WINDOW_HEIGHT + JN_GameWorld::BANNER_HEIGHT - PLAYER_HEIGHT, fmax(JN_GameWorld::BANNER_HEIGHT + 5, newRect.y)));

	rect.x = newRect.x;
	rect.y = newRect.y;
}


std::set<JN_Player::SpriteType> JN_Player::GetColliders(std::vector<JN_Sprite*> tiles)
{
	std::set<SpriteType> colliders = {};	// We only care about each type, not quantity

	for (int i = 0; i < (int)tiles.size(); i++)
	{
		// Spent about an hour trying to fix this, I had + 1 instead of + i
		if (Collide(rect, tiles[i]->rect))
			colliders.insert(tiles[i]->type);
	}
	return colliders;
}


void JN_Player::Render(SDL_Renderer *renderer)
{
	RenderPlayerHealthBar(renderer);
	JN_Sprite::Render(renderer);
	projectileController.Render(renderer);

}


void JN_Player::AnimationUpdate()
{
	float now = (float)SDL_GetTicks();

	if (now - lastSpriteChange > spriteChangeDelay)
	{
		lastSpriteChange = now;
		if (controls.GetKeyboardPresses().size() > 0)
			spriteIndex = spriteIndex == 1 ? 2 : 1;
		else
			spriteIndex = 0;
	}
}


void JN_Player::RenderPlayerHealthBar(SDL_Renderer *renderer)
{
	SDL_Rect r;
	r.w = (int)fmax(fmin(health.GetHealth(), 100), 1) * 2;
	r.x = 320 - (r.w / 2);
	r.h = 8;
	r.y = 5;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

	SDL_RenderFillRect(renderer, &r);
}