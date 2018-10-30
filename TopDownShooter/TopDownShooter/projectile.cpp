
#include "stdafx.h"
#include "projectile.h"
#include "gameWorld.h"


/* Header File Includes
	"square.h"
	vector2.h
	"sdl.h"
*/

#include <iostream>

void Projectile::Init(RGB _col)
{
	Square::Init(SquareType::PROJECTILE, 0, 0, _col);

	rect.w = PROJECTILE_WIDTH;
	rect.h = PROJECTILE_HEIGHT;
}

void Projectile::SetTarget(SDL_Rect _sourceRect, SDL_Rect _targetRect)
{
	visible = true;

	vector2 = Vector2((float)_targetRect.x, (float)_targetRect.y);

	x = y = 0;
	sourceRect.x = rect.x = _sourceRect.x;
	sourceRect.y = rect.y = _sourceRect.y;
}

void Projectile::Update()
{
	if (!visible) { return; }

	Vector2 v = vector2.Normalise();

	x += v.x;
	y += v.y;

	rect.x = (int)(x * 3);
	rect.y = (int)(y * 3);
}

bool Projectile::OutOfBounds()
{
	// Projectile out of screen
	return (rect.x < 0 || rect.y < 0) || (rect.x + rect.w > GameWorld::WINDOW_WIDTH || rect.y + rect.h > GameWorld::WINDOW_HEIGHT);
}

void Projectile::LateUpdate()
{
	if (!visible) { return; }

	if (OutOfBounds())
	{
		visible = false;
	}
}

void Projectile::Render(SDL_Renderer *renderer)
{
	if (visible)
	{
		Square::Render(renderer);
	}	
}