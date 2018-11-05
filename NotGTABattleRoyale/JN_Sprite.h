#ifndef JN_SPRITE_H
#define JN_SPRITE_H

#include "SDL.h"
#include "JN_Logging.h"	

#include <map>

struct SpriteAsset
{
	SpriteAsset() {}

	SpriteAsset(char *path) { this->path = path; }

	char *path;
	SDL_Surface *surface = NULL;
	SDL_Texture *texture = NULL;
};

class JN_Sprite
{
public:
	static const char EMPTY_TILE_CHAR           = '0';
	static const char MOVEMENT_DEBUFF_TILE_CHAR = '1';

	enum class SpriteType {MOVEMENT_DEBUFF, EMPTY, PLAYER, PROJECTILE};

	JN_Sprite();

	SpriteType type;
	SDL_Rect rect  = SDL_Rect();

	void virtual Init(SpriteType _type, SDL_Renderer *renderer, SDL_Rect _rect);	// Init the sprite
	void virtual Render(SDL_Renderer *renderer);									// Render the sprite onto the screen

	bool Collide(SDL_Rect a, SDL_Rect b);											// Checks if two rects are colliding
	float DistanceBetween(SDL_Rect a, SDL_Rect b);									// Returns the distance between two rects


	static SDL_Surface* LoadMedia(char *path);
	static SDL_Texture* Surface2Texture(SDL_Renderer *renderer, SDL_Surface *_surface);

protected:
	SDL_Surface *surface = NULL;
	SDL_Texture *texture = NULL;

	static std::map<SpriteType, SpriteAsset> assetsMap;
};

#endif // !JN_SPRITE_H
