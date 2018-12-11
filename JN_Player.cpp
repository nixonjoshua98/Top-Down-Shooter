#include "stdafx.h"

#include "JN_Player.h"
#include "JN_GameWorld.h"
//#include "JN_Enemy.h"

#include <algorithm>
#include <iostream>
#include <cmath>
#include <string>

// Default constructopr
JN_Player::JN_Player()
{

}


// De-constructor
JN_Player::~JN_Player()
{
	Mix_FreeChunk(pewpew);
	SDL_JoystickClose(gameController);

	logObj = NULL;
	windowData = NULL;
	gameController = NULL;
}


// Initilizes the player
void JN_Player::Init(SDL_Renderer *renderer, JN_Logging *logObj, JN_WindowData *windowData)
{
	pewpew = Mix_LoadWAV("Assets/pewPew.wav");
	JN_GameObject::Init(Tag::PLAYER, JN_GameObject::playerSpriteSheet.GetTexture(), rect, logObj);

	JN_RealTimer t = JN_RealTimer();
	damageTileTimer = JN_RealTimer();

	this->logObj = logObj;
	this->windowData = windowData;

	// Init objects
	projectileController.Init(Tag::PLAYER_PROJECTILE, 15, logObj, windowData);
	health.Init(100, logObj);
	controls.Init(logObj);
	animController.Init(200, GetTexture());

	// Add animations
	animController.Add(JN_AnimationController::Animation::IDLE, 0, PLAYER_WIDTH, PLAYER_HEIGHT, 1);
	animController.Add(JN_AnimationController::Animation::MOVING, 1, PLAYER_WIDTH, PLAYER_HEIGHT, 2);

	rect = SDL_Rect{ windowData->windowWidth / 2, windowData->windowHeight / 2, PLAYER_WIDTH, PLAYER_HEIGHT };
	newRect = rect;

	projectileController.CreateInitialProjectiles();
	animController.Set(JN_AnimationController::Animation::IDLE);

	gameController = SDL_JoystickOpen(0);

	logObj->LogTimeSpan("Player initilized", t.Tick());
}


// Input...
void JN_Player::Input(SDL_Event e)
{
	// Keyboard key has been released or pressed | check () for AND OR
	if ((e.type == SDL_KEYUP || e.type == SDL_KEYDOWN) && (controls.ValidControl(JN_PlayerControls::InputDevice::KEYBOARD, e.key.keysym.scancode)))
		KeyboardInputHandler(e);

	// Mouse button has been clicked or released
	else if ((e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) && (controls.ValidControl(JN_PlayerControls::InputDevice::MOUSE, e.button.button)))
		MouseInputHandler(e);

	else if ((e.type == SDL_JOYAXISMOTION || e.type == SDL_JOYBUTTONDOWN || e.type == SDL_JOYBUTTONUP) && (isUsingGamepad))
		GamepadInputHandler(e);
}


// Updates current keyboard presses
void JN_Player::KeyboardInputHandler(SDL_Event e)
{
	bool keyPressedDown = controls.IsKeyDown(JN_PlayerControls::InputDevice::KEYBOARD, e.key.keysym.scancode);

	// New key has been pressed
	if ((e.type == SDL_KEYDOWN) && !keyPressedDown)
		controls.AddKeyPress(JN_PlayerControls::InputDevice::KEYBOARD, e.key.keysym.scancode);

	// Old keypress has been lifted
	else if ((e.type == SDL_KEYUP) && keyPressedDown)
		controls.RemoveKeyPress(JN_PlayerControls::InputDevice::KEYBOARD, e.key.keysym.scancode);

	logObj->LogKeyboardInput(e.type == SDL_KEYDOWN, SDL_GetScancodeName(e.key.keysym.scancode));
}


// Updates current mouse presses
void JN_Player::MouseInputHandler(SDL_Event e)
{
	bool keyPressedDown = controls.IsKeyDown(JN_PlayerControls::InputDevice::MOUSE, e.button.button);

	// New mouse click
	if ((e.type == SDL_MOUSEBUTTONDOWN) && !keyPressedDown)
		controls.AddKeyPress(JN_PlayerControls::InputDevice::MOUSE, e.button.button);

	// Old mouse click has been lifted
	else if ((e.type == SDL_MOUSEBUTTONUP) && keyPressedDown)
		controls.RemoveKeyPress(JN_PlayerControls::InputDevice::MOUSE, e.button.button);

	logObj->LogMouseInput(e.type == SDL_MOUSEBUTTONDOWN, e.button.button == SDL_BUTTON_LEFT ? "Left" : "Right");
}


void JN_Player::GamepadInputHandler(SDL_Event e)
{
	if (e.type == SDL_JOYAXISMOTION)
	{
		gamepadVector.x = SDL_JoystickGetAxis(gameController, 0) / 3276.70f;
		gamepadVector.y = SDL_JoystickGetAxis(gameController, 1) / 3276.70f;

		if (abs(gamepadVector.x) <= 1.0f)
			gamepadVector.x = 0;

		if (abs(gamepadVector.y) <= 1.0f)
			gamepadVector.y = 0;
	}
	else if (e.jbutton.button == 5)
	{
		isGamepadShooting = !isGamepadShooting;
		logObj->LogGamepadInput("Shoot");
	}
}


void JN_Player::Shoot()
{
	if ((velocity.x == 0 && velocity.y == 0) && (gamepadVector.x == 0 && gamepadVector.y == 0))
		return;

	float now = (float)SDL_GetTicks();

	bool readyToShoot = (now - lastShootTime > SHOOT_DELAY);
	bool triggerDown = controls.IsKeyDown(JN_PlayerControls::InputDevice::MOUSE, JN_PlayerControls::ControlAction::SHOOT) || isGamepadShooting;

	if (!readyToShoot || !triggerDown)
		return;

	// Setup the target rect
	SDL_Rect target = SDL_Rect();

	int x, y;	// Get the mouse position
	if (!isUsingGamepad)
	{
		SDL_GetMouseState(&x, &y);
		target.x = x - rect.x;
		target.y = y - rect.y;
	}
	else
	{
		if (rotationVector.x == 0 && rotationVector.y == 0)
			return;

		target.x = (rect.x + rotationVector.x) - rect.x;
		target.y = (rect.y + rotationVector.y) - rect.y;
	}

	if (projectileController.Shoot(rect, target))
	{
		if (!JN_GameWorld::isSfxMute)
			Mix_PlayChannel(1, pewpew, 0);
		lastShootTime = now;
	}
}


void JN_Player::Update()
{
	isUsingGamepad = controls.GetKeyboardPresses().empty();

	projectileController.Update();
	Move();
	RotatePlayer();
	Shoot();
	animController.Update();
}


void JN_Player::UpdateAnimation()
{
	if (isAbleToMove && isMoving)
		animController.Set(JN_AnimationController::Animation::MOVING);

	else if (isAbleToMove && !isMoving)
		animController.Set(JN_AnimationController::Animation::IDLE);
}


void JN_Player::Move()
{
	float now = (float)SDL_GetTicks();

	isAbleToMove = false;

	if (now - lastMovementTime < MOVEMENT_DELAY)
		return;

	isAbleToMove = true;

	lastMovementTime = now;

	newRect = SDL_Rect{ rect.x, rect.y, rect.w, rect.h};

	velocity.x = 0;
	velocity.y = 0;

	float movementMultiplier = (isSlowed ? (1.0f * MOVEMENT_TILE_MULTIPLIER) : 1.0f)  * speedControl;

	for (JN_PlayerControls::ControlAction key : controls.GetKeyboardPresses())
	{
		switch (key)
		{
		case JN_PlayerControls::ControlAction::LEFT:
			velocity.x -= (int)(MOVEMENT_SPEED * movementMultiplier);
			break;

		case JN_PlayerControls::ControlAction::RIGHT:
			velocity.x += (int)(MOVEMENT_SPEED * movementMultiplier);
			break;

		case JN_PlayerControls::ControlAction::UP:
			velocity.y -= (int)(MOVEMENT_SPEED * movementMultiplier);
			break;

		case JN_PlayerControls::ControlAction::DOWN:
			velocity.y += (int)(MOVEMENT_SPEED * movementMultiplier);
			break;

		case JN_PlayerControls::ControlAction::SPEED_UP:
			speedControl = (float)std::fmin(speedControl + 0.15f, 1.0f);
			break;

		case JN_PlayerControls::ControlAction::SPEED_DOWN:
			speedControl = (float)std::fmax(speedControl - 0.15f, 0.0f);
			break;

		}
	}

	if (abs(velocity.x) > abs((MOVEMENT_SPEED * movementMultiplier) * (gamepadVector.x / 10)))
		newRect.x += velocity.x;
	else
		newRect.x += (MOVEMENT_SPEED * movementMultiplier) * (gamepadVector.x / 10);


	if (abs(velocity.y) > abs((MOVEMENT_SPEED * movementMultiplier) * (gamepadVector.y / 10)))
		newRect.y += velocity.y;
	else
		newRect.y += (MOVEMENT_SPEED * movementMultiplier) * (gamepadVector.y / 10);

	logObj->LogVelocity(newRect.x - rect.x, newRect.y - rect.y);
}


void JN_Player::RotatePlayer()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	if (isUsingGamepad)
	{
		x = SDL_JoystickGetAxis(gameController, 3) / 3276.70f;
		y = SDL_JoystickGetAxis(gameController, 4) / 3276.70f;
		rotationAngle = (float)(atan2((newRect.y - y) - newRect.y, (newRect.x - x) - newRect.x) * 180.0f / 3.14159) + 180.0f;

		rotationVector.x = x;
		rotationVector.y = y;
	}
	else
	{
		rotationAngle = (float)(atan2((y - 2) - newRect.y, (x - 2) - newRect.x) * 180.0f / 3.14159);
	}
}


void JN_Player::LateUpdate(std::vector<JN_GameObject*> tiles, std::vector<JN_Enemy*> enemies)
{
	ConfirmPlayerMovement();
	UpdateAnimation();
	ColliderManager(tiles);
	projectileController.LateUpdate(enemies);
}


void JN_Player::ColliderManager(std::vector<JN_GameObject*> tiles)
{
	ResetBuffs();

	// Current colliders
	for (auto c : GetColliders(tiles))
	{
		switch (c)
		{
		case Tag::MOVEMENT_DEBUFF:
			isSlowed = true;
			logObj->LogMethod("Player collided with movement debuff tile");
			break;

		case Tag::DAMAGE:
			if (damageTileTimer.Tick() >= 250)
			{
				damageTileTimer.Reset();
				logObj->LogMethod("Player collided with damage tile and took damage");
				health.TakeDamage(DAMAGE_TILE_AMOUNT);
			}
			break;
		}
	}
}


void JN_Player::ResetBuffs()
{
	isSlowed = false;
}


void JN_Player::ConfirmPlayerMovement()
{
	isMoving = false;

	// Makes sure that the player is always within the screen boundaries
	newRect.x = (int)(fmin(windowData->xOffset + JN_GameWorld::MIN_WINDOW_WIDTH - rect.w, fmax(newRect.x, windowData->xOffset + (rect.w / 2))));
	newRect.y = (int)(fmin(windowData->yOffset + JN_GameWorld::MIN_WINDOW_HEIGHT - rect.h, fmax(newRect.y, windowData->yOffset + (rect.h / 2))));

	if ((newRect.x != rect.x) || (newRect.y != rect.y))
	{
		isMoving = true;
		rect = newRect;
	}
}


std::set<JN_Player::Tag> JN_Player::GetColliders(std::vector<JN_GameObject*> tiles)
{
	std::set<Tag> colliders = {};	// We only care about each type, not quantity

	for (int i = 0; i < (int)tiles.size(); i++)
	{
		// Spent about an hour trying to fix this, I had + 1 instead of + i
		if (Collide(tiles[i]->rect))
			colliders.insert(tiles[i]->tag);
	}
	return colliders;
}


void JN_Player::Render(SDL_Renderer *renderer)
{
	animController.Render(rect, rotationAngle, renderer);
	projectileController.Render(renderer);
}


void JN_Player::EmptyInput()
{
	controls.EmptyInput();
}


void JN_Player::Resize(int xOffset, int yOffset)
{
	JN_GameObject::Resize(xOffset, yOffset);
	projectileController.Resize(xOffset, yOffset);

	newRect.x += xOffset;
	newRect.y += yOffset;
}


int JN_Player::GetHealth()
{
	return health.GetHealth();
}


int JN_Player::GetScore()
{
	return score;
}


void JN_Player::TakeDamage(int dmg)
{
	health.TakeDamage(dmg);
}


void JN_Player::AddScore(int s)
{
	score += s;
}

int JN_Player::GetDamageTaken()
{
	return health.GetDamageTaken();
}