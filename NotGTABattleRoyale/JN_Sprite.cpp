#include "stdafx.h"

#include "JN_Sprite.h"
#include "JN_Logging.h"

#include <iostream>

std::map<JN_Sprite::SpriteType, SpriteAsset> JN_Sprite::assetsMap = {
	{ SpriteType::EMPTY,           SpriteAsset("Assets/EmptyTile.BMP") },
	{ SpriteType::MOVEMENT_DEBUFF, SpriteAsset("Assets/MovementDebuffTile.BMP") },
	{ SpriteType::PLAYER,          SpriteAsset("Assets/Player.BMP") },
	{ SpriteType::PROJECTILE,      SpriteAsset("Assets/Projectile.BMP") },
};

JN_Sprite::JN_Sprite()
{

}

void JN_Sprite::Init(SpriteType _type, SDL_Renderer *renderer, SDL_Rect _rect)
{
	this->type = _type;

	if (JN_Sprite::assetsMap[this->type].surface == NULL)
		JN_Sprite::assetsMap[this->type].surface = LoadMedia(assetsMap[this->type].path);

	if (JN_Sprite::assetsMap[this->type].texture == NULL)
		JN_Sprite::assetsMap[this->type].texture = Surface2Texture(renderer, assetsMap[this->type].surface);

	this->rect    = _rect;
	this->surface = JN_Sprite::assetsMap[this->type].surface;
	this->texture = JN_Sprite::assetsMap[this->type].texture;
}

SDL_Surface* JN_Sprite::LoadMedia(char *path)
{
	return SDL_LoadBMP(path);
}

SDL_Texture* JN_Sprite::Surface2Texture(SDL_Renderer *renderer, SDL_Surface *_surface)
{
	return SDL_CreateTextureFromSurface(renderer, _surface);
}

void JN_Sprite::Render(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, this->texture, NULL, &this->rect);
}

bool JN_Sprite::Collide(SDL_Rect a, SDL_Rect b)
{
	int leftA = a.x, rightA = a.x + a.w, topA = a.y, bottomA = a.y + a.h;
	int leftB = b.x, rightB = b.x + b.w, topB = b.y, bottomB = b.y + b.h;

	return !(bottomA <= topB || topA >= bottomB || rightA <= leftB || leftA >= rightB);
}

float JN_Sprite::DistanceBetween(SDL_Rect a, SDL_Rect b)
{
	return (float)sqrt(pow(a.x - b.x, 2) + (pow(a.y - b.y, 2)));
}