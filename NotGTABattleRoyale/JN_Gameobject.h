#ifndef JN_GAMEOBJECT_H
#define JN_GAMEOBJECT_H

#include "JN_Logging.h"	
#include "JN_Asset.h"

#include <map>

class JN_Gameobject
{
public:
	///<summary>Default constructor</summary>
	JN_Gameobject();


	///<summary>De-constructor</summary>
	~JN_Gameobject();

	static JN_Asset tileSheet;
	static JN_Asset playerProjectile;
	static JN_Asset playerSpriteSheet;

	SDL_Rect MOVEMENT_DEBUFF_TILE_RECT	= SDL_Rect{ 0,  0, 64, 64 };
	SDL_Rect EMPTY_TILE_RECT			= SDL_Rect{ 64, 0, 64, 64 };
	SDL_Rect DAMAGE_TILE_RECT			= SDL_Rect{ 128, 0, 64, 64 };

	static const int TOTAL_TILES = 3;

	// Possible game object types
	enum class Tag { MOVEMENT_DEBUFF, EMPTY, PLAYER, PLAYER_PROJECTILE, DAMAGE };

	Tag tag;
	SDL_Rect rect;
	float rotationAngle = 0.0f;

	void Init(Tag tag, SDL_Texture* texture, SDL_Rect rect, JN_Logging *logObj);
	void Init(Tag tag, SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect dstRect, JN_Logging* logObj);


	///<summary>Renders the sprite to the screen</summary>
	///<param name = "renderer">The renderer which the sprite will be on</param>
	void virtual Render(SDL_Renderer *renderer);


	///<summary>Relocates the object based on the params</summary>
	///<param name = "x">X offset which will be added ot rect.x</param>
	///<param name = "y">Y offset which will be added ot rect.y</param>
	void virtual Resize(int x, int y);


	SDL_Texture* GetTexture();

protected:
	SDL_Rect srcRect;

	SDL_Surface *surface = NULL;	// Not sure why i need the surface but if i do not, the projectiles don;t move for some reason
	SDL_Texture *texture = NULL;	// ...
	JN_Logging *logObj = NULL;


	///<summary>Checks if two rects are colliding</summary>
	///<param name = "collider">Rect the object is checking against</param>
	bool Collide(SDL_Rect collider);
};

#endif // !JN_GAMEOBJECT_H
