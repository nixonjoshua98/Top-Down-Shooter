#include "stdafx.h"

#include "JN_Projectile.h"
#include "JN_GameWorld.h"

#include <iostream>

JN_Projectile::~JN_Projectile()
{
	logObj = NULL;
	windowData = NULL;
}

void JN_Projectile::Init(JN_Gameobject::Tag tag, SDL_Renderer *renderer, JN_Logging *logObj, JN_WindowData *windowData)
{
	this->logObj = logObj;
	this->windowData = windowData;

	JN_Gameobject::Init(tag, renderer, SDL_Rect(), logObj);

	rect.w = PROJECTILE_WIDTH;
	rect.h = PROJECTILE_HEIGHT;
}

void JN_Projectile::SetTarget(SDL_Rect sourceRect, SDL_Rect targetRect)
{
	visible = true;
	this->sourceRect = sourceRect;
	vector2 = JN_Vector2((float)targetRect.x, (float)targetRect.y);
	x = (float)sourceRect.x, y = (float)sourceRect.y;

	rect.x = this->sourceRect.x;
	rect.y = this->sourceRect.y;
}

void JN_Projectile::Update()
{
	if (visible)
	{
		JN_Vector2 v = vector2.Normalise();

		x += v.x * speedMultiplier;
		y += v.y * speedMultiplier;

		rect.x = (int)x;
		rect.y = (int)y;
	}
}

bool JN_Projectile::OutOfBounds()
{
	// Projectile out of screen - Split over two lines as very long
	bool x = ((rect.x + rect.w) < windowData->xOffset || (rect.x > (windowData->xOffset + JN_GameWorld::MIN_WINDOW_WIDTH)));
	bool y = (rect.y < (windowData->yOffset) || (rect.y > (windowData->yOffset + JN_GameWorld::MIN_WINDOW_HEIGHT)));

	return x || y;
}

void JN_Projectile::LateUpdate()
{
	if (visible && OutOfBounds())
		visible = false;
}

void JN_Projectile::Render(SDL_Renderer *renderer)
{
	if (visible)
		JN_Gameobject::Render(renderer);
}