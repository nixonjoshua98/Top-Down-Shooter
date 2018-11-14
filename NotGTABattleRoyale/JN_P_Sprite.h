#ifndef JN_P_SPRITE_H
#define JN_P_SPRITE_H

#include <SDL.h>

class JN_P_Sprite
{
public:
	///<summary>Default constructor</summary>
	JN_P_Sprite();


	///<summary>Constructor overload</summary>
	///<param name = "renderer">Main renderer</param>
	///<param name = "path">Path to the BMP file</param>
	JN_P_Sprite(SDL_Renderer *renderer, char *path);


	///<summary>Constructor overload</summary>
	///<param name = "surface">Sprite surface</param>
	///<param name = "texture">Texture of the sprite</param>
	JN_P_Sprite(SDL_Surface *surface, SDL_Texture *texture);


	///<summary>Non-constructor initilization</summary>
	///<param name = "renderer">Main renderer</param>
	///<param name = "path">Path to the BMP file</param>
	void Init(SDL_Renderer *renderer, char *path);


	///<summary>Non-constructor initilization</summary>
	///<param name = "surface">Sprite surface</param>
	///<param name = "texture">Texture of the sprite</param>
	void Init(SDL_Surface *surface, SDL_Texture *texture);


	///<summary>De-constructor</summary>
	~JN_P_Sprite();


	///<summary>Reads a sprite at path and returns the surface</summary>
	SDL_Surface* LoadMedia();


	///<summary>Returns a texture based of the surface member</summary>
	SDL_Texture* CreateTextureFromSurface(SDL_Renderer *renderer);

private:
	char *pathToAsset;				// ...

	SDL_Surface *surface = NULL;	// ...
	SDL_Texture *texture = NULL;	// ...
};

#endif // !JN_P_SPRITE_H
