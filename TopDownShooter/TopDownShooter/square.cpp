
#include "stdafx.h"
#include "square.h"
#include "gameWorld.h"

#include <iostream>

/* Header File Includes
	"sdl.h"
*/

void Square::Init(SquareType type, int x, int y, RGB _col)
{
	rect       = SDL_Rect();
	squareType = type;

	col    = _col;
	rect.x = x;
	rect.y = y;
	rect.w = GameWorld::CELL_WIDTH;
	rect.h = GameWorld::CELL_HEIGHT;
}

void Square::Render(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, col.R, col.G, col.B, 255);
	//SDL_RenderFillRect(renderer, &rect);
	SDL_RenderDrawRect(renderer, &rect);
}

bool Square::Collide(SDL_Rect a, SDL_Rect b)
{
	/*
	Surely this could be improved upon?
	TODO: Reduce the lines of code
	*/

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

float Square::DistanceBetween(SDL_Rect a, SDL_Rect b)
{
	return (float)sqrt(pow(a.x - b.x, 2) + (pow(a.y - b.y, 2)));
}