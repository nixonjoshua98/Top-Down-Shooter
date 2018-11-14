#ifndef JN_SPRITE_ASSET_H
#define JN_SPRITE_ASSET_H

#include <SDL.h>

struct JN_SpriteAsset
{
	///<summary>Default constructor</summary>
	JN_SpriteAsset()
	{

	}


	///<summary>Constructor overload</summary>
	///<param name = "path">Path to the asset being 'saved'</param>
	JN_SpriteAsset(char *path)
	{
		this->path = path;
	}


	///<summary>De-constructor</summary>
	~JN_SpriteAsset()
	{
		SDL_FreeSurface(this->surface);
		SDL_DestroyTexture(this->texture);
	}

	char *path;						// Path to sprite
	SDL_Surface *surface = NULL;	// Pointer ot surface of the sprite
	SDL_Texture *texture = NULL;	// texture of the surface sprite
};

#endif // !JN_SPRITE_ASSET_H
