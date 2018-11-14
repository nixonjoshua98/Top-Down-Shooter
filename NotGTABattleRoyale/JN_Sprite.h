#ifndef JN_SPRITE_H
#define JN_SPRITE_H

#include "JN_Logging.h"	
#include "JN_SpriteAsset.h"

#include <map>

class JN_Sprite
{
public:
	int _ = 0;

	static const char EMPTY_TILE_CHAR           = '0';
	static const char MOVEMENT_DEBUFF_TILE_CHAR = '1';
	static const char FIRE_DAMAGE_TILE_CHAR     = '2';

	static const int TOTAL_TILE_TYPES = 3;

	static const int FIRE_DAMAGE			  = 1;	// 1 Damage every frame the user touches the collider
	static const int MOVEMENT_DEBUFF_AMOUNT   = 2;	// Movement will be halved

	enum class SpriteType {MOVEMENT_DEBUFF, EMPTY, PLAYER, PROJECTILE, FIRE_DAMAGE};

	JN_Sprite();
	~JN_Sprite();

	SpriteType type;
	SDL_Rect rect;


	void virtual Init(SpriteType _type, SDL_Renderer *renderer, SDL_Rect _rect, JN_Logging *logObj, int _totalSprites);	// Init the sprite


	///<summary>Renders the sprite to the screen</summary>
	///<param name = "renderer">The renderer which the sprite will be on</param>
	void virtual Render(SDL_Renderer *renderer);


	///<summary>Relocates the object based on the params</summary>
	///<param name = "x">X offset which will be added ot rect.x</param>
	///<param name = "y">Y offset which will be added ot rect.y</param>
	void Resize(int x, int y);

	// Static methods
	SDL_Surface* LoadMedia(char *path, bool makeTrans = true);
	SDL_Texture* Surface2Texture(SDL_Renderer *renderer, SDL_Surface *_surface);

protected:
	SDL_Surface *surface = NULL;
	SDL_Texture *texture = NULL;

	int spriteIndex = 0;

	JN_Logging *logObj = NULL;

	// Delays
	float spriteChangeDelay = 250;

	int totalSprites;

	// Delay timers
	float lastSpriteChange  = (float)SDL_GetTicks();

	static std::map<SpriteType, JN_SpriteAsset*> assetsMap;

	double rotationAngle = 0;

	bool Collide(SDL_Rect a, SDL_Rect b);			// Checks if two rects are colliding
	float DistanceBetween(SDL_Rect a, SDL_Rect b);	// Returns the distance between two rects
};

#endif // !JN_SPRITE_H
