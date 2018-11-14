#include "stdafx.h"

#include "JN_P_Sprite.h"

// Default constructor
JN_P_Sprite::JN_P_Sprite()
{

}


// Constructor overload
JN_P_Sprite::JN_P_Sprite(SDL_Renderer *renderer, char *path)
{
	pathToAsset = path;
	surface = LoadMedia();
	texture = CreateTextureFromSurface(renderer);
}


// Constructor overload
JN_P_Sprite::JN_P_Sprite(SDL_Surface *surface, SDL_Texture *texture)
{
	this->surface = surface;
	this->texture = texture;
}


// De-constructor
JN_P_Sprite::~JN_P_Sprite()
{
	texture = NULL;
	surface = NULL;
	pathToAsset = NULL;	// Not needed
}


// Non-constructor initilization
void JN_P_Sprite::Init(SDL_Renderer *renderer, char *path)
{
	JN_P_Sprite(renderer, path);
}


// Non-constructor initilization
void JN_P_Sprite::Init(SDL_Surface *surface, SDL_Texture *texture)
{
	JN_P_Sprite(surface, texture);
}


// Read and return a sprite surface
SDL_Surface* JN_P_Sprite::LoadMedia()
{
	SDL_Surface *img = SDL_LoadBMP(pathToAsset);

	// -> Transparant
	SDL_SetColorKey(img, 1, SDL_MapRGB(img->format, 255, 255, 255));

	return img;
}


// Returns a tetxure of the surface member
SDL_Texture* JN_P_Sprite::CreateTextureFromSurface(SDL_Renderer *renderer)
{
	return SDL_CreateTextureFromSurface(renderer, surface);
}