#include "stdafx.h"

#include "JN_Sprite.h"


std::map<JN_Gameobject::Tag, JN_SpriteData*> JN_Sprite::assetsDataMap = {
	{ JN_Gameobject::Tag::EMPTY,				new JN_SpriteData("Assets/FloorTile.BMP") },
	{ JN_Gameobject::Tag::MOVEMENT_DEBUFF,		new JN_SpriteData("Assets/MovementDebuffTile.BMP") },
	{ JN_Gameobject::Tag::PLAYER,				new JN_SpriteData("Assets/PlayerSpritesheet.BMP") },
	{ JN_Gameobject::Tag::PLAYER_PROJECTILE,	new JN_SpriteData("Assets/Projectile.BMP") },
	{ JN_Gameobject::Tag::DAMAGE,				new JN_SpriteData("Assets/FireDamageTile.BMP") },
};

// Default constructor
JN_Sprite::JN_Sprite()
{

}


JN_Sprite::~JN_Sprite()
{
	texture = NULL;
	surface = NULL;
}


void JN_Sprite::Init(JN_Gameobject::Tag tag, SDL_Renderer *renderer)
{
	if (assetsDataMap[tag]->surface == NULL)
		assetsDataMap[tag]->surface = LoadMedia(tag);

	if (assetsDataMap[tag]->texture == NULL)
		assetsDataMap[tag]->texture = Surface2Texture(renderer);

	this->surface = assetsDataMap[tag]->surface;
	this->texture = Surface2Texture(renderer);
}


SDL_Surface* JN_Sprite::LoadMedia(JN_Gameobject::Tag tag)
{
	SDL_Surface *img = SDL_LoadBMP(assetsDataMap[tag]->path);

	SDL_SetColorKey(img, 1, SDL_MapRGB(img->format, 255, 255, 255));

	return img;
}


SDL_Texture* JN_Sprite::Surface2Texture(SDL_Renderer *renderer)
{
	return SDL_CreateTextureFromSurface(renderer, surface);
}


SDL_Texture* JN_Sprite::GetTexture()
{
	return texture;
}