#ifndef JN_SPRITE_H
#define JN_SPRITE_H

#include "SDL.h"
#include "JN_Logging.h"	

#include <map>

struct SpriteAsset
{
	SpriteAsset() {}
	SpriteAsset(char *path) { this->path = path; }

	~SpriteAsset()
	{
		SDL_FreeSurface(this->surface);
		SDL_DestroyTexture(this->texture);
	}

	char *path;
	SDL_Surface *surface = NULL;
	SDL_Texture *texture = NULL;
};





class JN_Sprite
{
public:
	static const char EMPTY_TILE_CHAR           = '0';
	static const char MOVEMENT_DEBUFF_TILE_CHAR = '1';
	static const char FIRE_DAMAGE_TILE_CHAR     = '2';

	static const int FIRE_DAMAGE			  = 1;	// 1 Damage every frame the user touches the collider
	static const int MOVEMENT_DEBUFF_AMOUNT   = 2;	// Movement will be halved

	enum class SpriteType {MOVEMENT_DEBUFF, EMPTY, PLAYER, PROJECTILE, FIRE_DAMAGE};

	JN_Sprite();
	~JN_Sprite();

	SpriteType type;
	SDL_Rect rect  = SDL_Rect();

	void virtual Init(SpriteType _type, SDL_Renderer *renderer, SDL_Rect _rect, int _totalSprites);	// Init the sprite
	void virtual Render(SDL_Renderer *renderer);													// Render the sprite onto the screen

	// Static methods
	static SDL_Surface* LoadMedia(char *path, bool makeTrans = true);
	static SDL_Texture* Surface2Texture(SDL_Renderer *renderer, SDL_Surface *_surface);

protected:
	SDL_Surface *surface = NULL;
	SDL_Texture *texture = NULL;

	int spriteIndex = 0;

	// Delays
	float spriteChangeDelay = 150;

	int totalSprites;

	// Delay timers
	float lastSpriteChange  = (float)SDL_GetTicks();

	static std::map<SpriteType, SpriteAsset*> assetsMap;

	double rotationAngle = 0;

	bool Collide(SDL_Rect a, SDL_Rect b);			// Checks if two rects are colliding
	float DistanceBetween(SDL_Rect a, SDL_Rect b);	// Returns the distance between two rects
};

#endif // !JN_SPRITE_H
