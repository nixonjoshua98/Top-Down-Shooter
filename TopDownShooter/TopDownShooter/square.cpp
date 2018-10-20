#include "stdafx.h"
#include "square.h"
#include "gameWorld.h"

#include <iostream>

void Square::Init(int x, int y, RGB _col)
{
	rect = SDL_Rect();

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