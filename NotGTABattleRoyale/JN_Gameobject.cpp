#include "stdafx.h"

#include "JN_GameObject.h"
#include "JN_RealTimer.h"
#include "JN_Logging.h"

#include <iostream>
#include <SDL.h>

JN_Asset JN_Gameobject::tileSheet = JN_Asset();
JN_Asset JN_Gameobject::playerProjectile = JN_Asset();
JN_Asset JN_Gameobject::playerSpriteSheet = JN_Asset();


JN_Gameobject::JN_Gameobject()
{
	
}


JN_Gameobject::~JN_Gameobject()
{
	JN_Gameobject::tileSheet.Cleanup();
	JN_Gameobject::playerProjectile.Cleanup();
	JN_Gameobject::playerSpriteSheet.Cleanup();

	texture = NULL;
	surface = NULL;
}


// Whole images
void JN_Gameobject::Init(Tag tag, SDL_Texture* texture, SDL_Rect rect, JN_Logging *logObj)
{
	this->tag = tag;
	this->rect = rect;
	this->logObj = logObj;
	this->texture = texture;

	SDL_QueryTexture(texture, NULL, NULL, &srcRect.w, &srcRect.h);
}


// Tile from the TileSheet
void JN_Gameobject::Init(Tag tag, SDL_Texture* texture, SDL_Rect dstRect, SDL_Rect srcRect, JN_Logging* logObj)
{
	this->tag = tag;
	this->rect = dstRect;
	this->logObj = logObj;
	this->texture = texture;
	this->srcRect = srcRect;
}


void JN_Gameobject::Render(SDL_Renderer *renderer)
{
	SDL_RenderCopyEx(renderer, GetTexture(), &srcRect, &rect, rotationAngle, NULL, SDL_FLIP_NONE);
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
	return texture;
}