#ifndef JN_SPRITE_H
#define JN_SPRITE_H

#include "JN_Gameobject.h"

#include <SDL.h>

class JN_Sprite
{
public:
	///<summary>Default constructor</summary>
	JN_Sprite();


	///<summary>De-constructor</summary>
	~JN_Sprite();


	// Map which stores the BMP files of the sprites, used as storage
	static std::map<JN_Gameobject::Tag, JN_SpriteData*> assetsDataMap;	// Assets storage map


	///<summary>Initiliisation</summary>
	///<param name = "tag">The type of sprite which will dictate which surface is used</param>
	///<param name = "renderer">Renderer used to create texture</param>
	void Init(JN_Gameobject::Tag tag, SDL_Renderer *renderer);


	///<summary>Load sprite and return surface</summary>
	SDL_Surface* LoadMedia(JN_Gameobject::Tag tag);


	///<summary>Return a tetxure from a surface</summary>
	///<param name = "renderer">Renderer used to create texture</param>
	SDL_Texture* Surface2Texture(SDL_Renderer *renderer);


	///<summary>Return the texture</summary>
	SDL_Texture* GetTexture();

private:
	SDL_Surface *surface = NULL;	// ...
	SDL_Texture *texture = NULL;	// ...
};


#endif // !JN_SPRITE_H
