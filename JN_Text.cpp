#include "stdafx.h"

#include "JN_Text.h"

#include <iostream>

JN_Text::JN_Text()
{

}


JN_Text::~JN_Text()
{
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);

	font = NULL;
	surface = NULL;
	texture = NULL;
}


void JN_Text::Init(int x, int y, int w, int h, SDL_Color col, char *fontPath, int size)
{
	this->col = col;

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;

	font = TTF_OpenFont(fontPath, size);
}

void JN_Text::Render(SDL_Renderer *renderer, std::string txt)
{
	ClearText();

	surface = TTF_RenderText_Solid(font, txt.c_str(), col);
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_RenderCopy(renderer, texture, NULL, &rect);
}

void JN_Text::ClearText()
{
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}


void JN_Text::Move(int xChange, int yChange)
{
	rect.x += xChange;
	rect.y += yChange;
}