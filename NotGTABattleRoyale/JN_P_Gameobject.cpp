#include "stdafx.h"

#include "JN_P_Gameobject.h"

// Default constructor
JN_P_Gameobject::JN_P_Gameobject()
{

}


// Initilizer
void JN_P_Gameobject::Init(Tag tag, SDL_Rect rect, SDL_Surface *surface, SDL_Texture *texture)
{
	this->tag = tag;
	this->rect = rect;
	this->sprite = JN_P_Sprite(surface, texture);
}


// Collision check between two rects
bool JN_P_Gameobject::Collide(SDL_Rect collider)
{
	int leftA = rect.x    , rightA = rect.x + rect.w        , topA = rect.y    , bottomA = rect.y + rect.h;
	int leftB = collider.x, rightB = collider.x + collider.w, topB = collider.y, bottomB = collider.y + collider.h;

	return !(bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB);
}