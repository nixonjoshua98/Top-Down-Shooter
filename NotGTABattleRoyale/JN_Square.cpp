
#include "stdafx.h"

#include "JN_Square.h"
#include "JN_GameWorld.h"

#include <iostream>

void JN_Square::Init(SquareType type, int x, int y, JN_RGB _col)
{
	rect       = SDL_Rect();
	squareType = type;

	col    = _col;
	rect.x = x;
	rect.y = y;
	rect.w = JN_GameWorld::CELL_WIDTH;
	rect.h = JN_GameWorld::CELL_HEIGHT;
}

void JN_Square::Render(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, col.R, col.G, col.B, 255);
	//SDL_RenderFillRect(renderer, &rect);
	SDL_RenderDrawRect(renderer, &rect);
}

bool JN_Square::Collide(SDL_Rect a, SDL_Rect b)
{
	int leftA = a.x, rightA = a.x + a.w, topA = a.y, bottomA = a.y + a.h;
	int leftB = b.x, rightB = b.x + b.w, topB = b.y, bottomB = b.y + b.h;

	return !(bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB);
}

float JN_Square::DistanceBetween(SDL_Rect a, SDL_Rect b)
{
	return (float)sqrt(pow(a.x - b.x, 2) + (pow(a.y - b.y, 2)));
}