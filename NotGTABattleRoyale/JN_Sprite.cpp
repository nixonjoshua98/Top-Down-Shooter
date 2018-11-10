#include "stdafx.h"

#include "JN_Sprite.h"
#include "JN_Logging.h"

#include <iostream>

std::map<JN_Sprite::SpriteType, SpriteAsset*> JN_Sprite::assetsMap = {
	{ SpriteType::EMPTY,           new SpriteAsset("Assets/FloorTile.BMP") },
	{ SpriteType::MOVEMENT_DEBUFF, new SpriteAsset("Assets/MovementDebuffTile.BMP") },
	{ SpriteType::PLAYER,          new SpriteAsset("Assets/PlayerSpritesheet.BMP") },
	{ SpriteType::PROJECTILE,      new SpriteAsset("Assets/Projectile.BMP") },
	{ SpriteType::FIRE_DAMAGE,     new SpriteAsset("Assets/FireDamageTile.BMP") },
};

JN_Sprite::JN_Sprite()
{
	// Default constructor
}

JN_Sprite::~JN_Sprite()
{
	// They get deallocated in the SpriteAsset
	surface = NULL;
	texture = NULL;
}

void JN_Sprite::Init(SpriteType _type, SDL_Renderer *renderer, SDL_Rect _rect, int _totalSprites)
{
	this->type = _type;

	// These few lines mean only the images and textures are created once for each image
	if (JN_Sprite::assetsMap[this->type]->surface == NULL)
		JN_Sprite::assetsMap[this->type]->surface = LoadMedia(assetsMap[this->type]->path);

	if (JN_Sprite::assetsMap[this->type]->texture == NULL)
		JN_Sprite::assetsMap[this->type]->texture = Surface2Texture(renderer, assetsMap[this->type]->surface);

	this->totalSprites = _totalSprites;

	this->rect         = _rect;
	this->originalRect = _rect;
	this->surface = JN_Sprite::assetsMap[this->type]->surface;
	this->texture = JN_Sprite::assetsMap[this->type]->texture;
}

SDL_Surface* JN_Sprite::LoadMedia(char *path, bool makeTrans)
{
	SDL_Surface *img =  SDL_LoadBMP(path);

	if (makeTrans)
		SDL_SetColorKey(img, 1, SDL_MapRGB(img->format, 255, 255, 255));

	return img;
}

SDL_Texture* JN_Sprite::Surface2Texture(SDL_Renderer *renderer, SDL_Surface *_surface)
{
	return SDL_CreateTextureFromSurface(renderer, _surface);
}

void JN_Sprite::Render(SDL_Renderer *renderer)
{
	if (totalSprites > 1)
	{
		SDL_Rect r;
		r.x = spriteIndex * rect.w;
		r.y = 0;
		r.w = rect.w;
		r.h = rect.h;

		SDL_RenderCopyEx(renderer, this->texture, &r, &this->rect, this->rotationAngle, NULL, SDL_FLIP_NONE);
	}
	else
		SDL_RenderCopyEx(renderer, this->texture, NULL, &this->rect, this->rotationAngle, NULL, SDL_FLIP_NONE);
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

void JN_Sprite::Resize(int x, int y, int w, int h)
{
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}