#include "stdafx.h"


#include "JN_Enemy.h"
#include "JN_Projectile.h"
#include "JN_GameWorld.h"

#include <iostream>

JN_Projectile::~JN_Projectile()
{
	logObj = NULL;
	windowData = NULL;
}

void JN_Projectile::Init(JN_GameObject::Tag tag, JN_Logging *logObj, JN_WindowData *windowData)
{
	this->logObj = logObj;
	this->windowData = windowData;

	rect.w = PROJECTILE_WIDTH;
	rect.h = PROJECTILE_HEIGHT;

	JN_GameObject::Init(tag, JN_GameObject::playerProjectile.GetTexture(), rect, logObj);
}

void JN_Projectile::SetTarget(SDL_Rect sourceRect, SDL_Rect targetRect)
{
	visible = true;
	this->sourceRect = sourceRect;
	vector2 = JN_Vector2((float)targetRect.x, (float)targetRect.y);

	x = (float)sourceRect.x;
	y = (float)sourceRect.y;

	rect.x = this->sourceRect.x;
	rect.y = this->sourceRect.y;

	rotationAngle = (float)(atan2((targetRect.y - 2) - sourceRect.y, (targetRect.x - 2) - sourceRect.x) * 180.0f / M_PI);
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

void JN_Projectile::LateUpdate(std::vector<JN_Enemy*> &enemies)
{
	if (visible && OutOfBounds())
		visible = false;

	if (!visible)
		return;

	for (int i = 0; i < (int)enemies.size(); i++)
	{
		if (Collide(enemies[i]->rect))
			enemies[i]->isDead = true;
	}
}

void JN_Projectile::Render(SDL_Renderer *renderer)
{
	if (visible)
		JN_GameObject::Render(renderer);
}