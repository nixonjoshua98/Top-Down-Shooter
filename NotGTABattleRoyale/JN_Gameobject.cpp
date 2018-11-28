#include "stdafx.h"

#include "JN_GameObject.h"
#include "JN_RealTimer.h"
#include "JN_Logging.h"

#include <iostream>
#include <SDL.h>

// Init static members
JN_Asset JN_GameObject::tileSheet = JN_Asset();
JN_Asset JN_GameObject::playerProjectile = JN_Asset();
JN_Asset JN_GameObject::playerSpriteSheet = JN_Asset();
JN_Asset JN_GameObject::scorpionSpriteSheet = JN_Asset();


JN_GameObject::JN_GameObject()
{
	
}


JN_GameObject::~JN_GameObject()
{
	JN_GameObject::tileSheet.Cleanup();
	JN_GameObject::playerProjectile.Cleanup();
	JN_GameObject::playerSpriteSheet.Cleanup();

	texture = NULL;
	surface = NULL;
}


// Whole images
void JN_GameObject::Init(Tag tag, SDL_Texture* texture, SDL_Rect rect, JN_Logging *logObj)
{
	this->tag = tag;
	this->rect = rect;
	this->logObj = logObj;
	this->texture = texture;

	SDL_QueryTexture(texture, NULL, NULL, &srcRect.w, &srcRect.h);
}


// Tile from the TileSheet
void JN_GameObject::Init(Tag tag, SDL_Texture* texture, SDL_Rect dstRect, SDL_Rect srcRect, JN_Logging* logObj)
{
	this->tag = tag;
	this->rect = dstRect;
	this->logObj = logObj;
	this->texture = texture;
	this->srcRect = srcRect;
}


void JN_GameObject::Render(SDL_Renderer *renderer)
{
	SDL_RenderCopyEx(renderer, GetTexture(), &srcRect, &rect, rotationAngle, NULL, SDL_FLIP_NONE);
}


bool JN_GameObject::Collide(SDL_Rect collider)
{
	int leftA = rect.x, rightA = rect.x + rect.w, topA = rect.y, bottomA = rect.y + rect.h;
	int leftB = collider.x, rightB = collider.x + collider.w, topB = collider.y, bottomB = collider.y + collider.h;

	return !(bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB);
}


void JN_GameObject::Resize(int x, int y)
{
	rect.x += x;
	rect.y += y;
}

SDL_Texture* JN_GameObject::GetTexture()
{
	return texture;
}