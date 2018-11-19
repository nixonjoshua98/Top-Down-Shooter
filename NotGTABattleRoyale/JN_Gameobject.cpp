#include "stdafx.h"

#include "JN_Sprite.h"
#include "JN_Gameobject.h"
#include "JN_RealTimer.h"
#include "JN_Logging.h"

#include <iostream>
#include <SDL.h>

std::map<char, JN_Gameobject::Tag> JN_Gameobject::charToTagMap = {
	{'0', Tag::EMPTY},
	{'1', Tag::MOVEMENT_DEBUFF },
	{'2', Tag::DAMAGE },
};


JN_Gameobject::JN_Gameobject()
{
	
}


JN_Gameobject::~JN_Gameobject()
{
	delete sprite;
}


void JN_Gameobject::Init(Tag tag, SDL_Renderer *renderer, SDL_Rect rect,  JN_Logging *logObj)
{
	sprite = new JN_Sprite();

	sprite->Init(tag, renderer);

	this->logObj = logObj;
	this->tag = tag;
	this->rect = rect;
}

void JN_Gameobject::Render(SDL_Renderer *renderer)
{
	SDL_RenderCopyEx(renderer, sprite->GetTexture(), NULL, &this->rect, this->rotationAngle, NULL, SDL_FLIP_NONE);
}

bool JN_Gameobject::Collide(SDL_Rect collider)
{
	int leftA = rect.x, rightA = rect.x + rect.w, topA = rect.y, bottomA = rect.y + rect.h;
	int leftB = collider.x, rightB = collider.x + collider.w, topB = collider.y, bottomB = collider.y + collider.h;

	return !(bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB);
}

void JN_Gameobject::Resize(int x, int y)
{
	rect.x += x;
	rect.y += y;
}

SDL_Texture* JN_Gameobject::GetTexture()
{
	return sprite->GetTexture();
}