#include "stdafx.h"
#include "projectile.h"
#include "gameWorld.h"

/* Header File Includes
	"square.h"
	"sdl.h"
*/

#include <iostream>

void Projectile::Init(int x, int y, int destX, int destY)
{
	Square::Init(x, y, RGB{ 255, 0, 0 });

	sourceRect.x = x;
	sourceRect.y = y;

	rect.w = 3;
	rect.h = 3;

	targetRect.x = destX;
	targetRect.y = destY;
}

void Projectile::Update()
{
	rect.x += (targetRect.x - sourceRect.x) * 0.015;
	rect.y += (targetRect.y - sourceRect.y) * 0.015;
}

bool Projectile::OutOfBounds()
{
	return (rect.x < 0 || rect.y < 0) || (rect.x + rect.w > GameWorld::WINDOW_WIDTH || rect.y + rect.h > GameWorld::WINDOW_HEIGHT);
}

