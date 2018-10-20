#include "stdafx.h"
#include "square.h"
#include "gameWorld.h"

#include <iostream>

void Square::Init(SquareType type, int x, int y, RGB _col)
{
	rect       = SDL_Rect();
	squareType = type;

	col    = _col;
	rect.x = x;
	rect.y = y;
	rect.w = GameWorld::WINDOW_WIDTH  / GameWorld::LEVEL_WIDTH;
	rect.h = GameWorld::WINDOW_HEIGHT / GameWorld::LEVEL_HEIGHT;
}

void Square::Render(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, col.R, col.G, col.B, 255);
	SDL_RenderFillRect(renderer, &rect);
}

bool Square::Collide(SDL_Rect a, SDL_Rect b)
{
	int leftA = a.x;
	int rightA = a.x + a.w;
	int topA = a.y;
	int bottomA = a.y + a.h;

	int leftB = b.x;
	int rightB = b.x + b.w;
	int topB = b.y;
	int bottomB = b.y + b.h;

	if (bottomA <= topB) { return false; }
	if (topA >= bottomB) { return false; }
	if (rightA <= leftB) { return false; }
	if (leftA >= rightB) { return false; }
	return true;
}