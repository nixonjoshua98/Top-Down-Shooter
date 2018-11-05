#include "stdafx.h"

#include "JN_Projectile.h"
#include "JN_GameWorld.h"

#include <iostream>

void JN_Projectile::Init(JN_RGB _col)
{
	JN_Square::Init(SquareType::PROJECTILE, 0, 0, _col);

	rect.w = PROJECTILE_WIDTH;
	rect.h = PROJECTILE_HEIGHT;
}

void JN_Projectile::SetTarget(SDL_Rect _sourceRect, SDL_Rect _targetRect)
{
	visible = true;
	sourceRect = _sourceRect;
	vector2 = JN_Vector2((float)_targetRect.x, (float)_targetRect.y);
	x = (float)_sourceRect.x, y = (float)_sourceRect.y;

	rect.x = _sourceRect.x;
	rect.y = _sourceRect.y;
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
	// Projectile out of screen - Splti over two lines as very long
	bool a = (rect.x < 0 || rect.y < JN_GameWorld::BANNER_HEIGHT);
	bool b = (rect.x + rect.w > JN_GameWorld::WINDOW_WIDTH || rect.y + rect.h > JN_GameWorld::WINDOW_HEIGHT);
	return a || b;
}

void JN_Projectile::LateUpdate()
{
	if (visible && OutOfBounds())
		visible = false;
}

void JN_Projectile::Render(SDL_Renderer *renderer)
{
	if (visible)
		JN_Square::Render(renderer);
}