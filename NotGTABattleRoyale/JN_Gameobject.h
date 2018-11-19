#ifndef JN_GAMEOBJECT_H
#define JN_GAMEOBJECT_H

#include "JN_Logging.h"	
#include "JN_SpriteData.h"

#include <map>


// Forward declarations (They now have to be a pointer)
class JN_Sprite;



class JN_Gameobject
{
public:
	///<summary>Default constructor</summary>
	JN_Gameobject();


	///<summary>De-constructor</summary>
	~JN_Gameobject();


	// Possible game object types
	enum class Tag { MOVEMENT_DEBUFF, EMPTY, PLAYER, PLAYER_PROJECTILE, DAMAGE };


	// Declaration can be found at the top of JN_Gameobject.cpp
	static std::map<char, Tag> charToTagMap;	// Char (text file) to tag

	JN_Sprite *sprite = NULL;

	Tag tag;
	SDL_Rect rect;
	float rotationAngle = 0.0f;

	void virtual Init(Tag tag, SDL_Renderer *renderer, SDL_Rect rect, JN_Logging *logObj);	// Init the sprite


	///<summary>Renders the sprite to the screen</summary>
	///<param name = "renderer">The renderer which the sprite will be on</param>
	void virtual Render(SDL_Renderer *renderer);


	///<summary>Relocates the object based on the params</summary>
	///<param name = "x">X offset which will be added ot rect.x</param>
	///<param name = "y">Y offset which will be added ot rect.y</param>
	void virtual Resize(int x, int y);

	SDL_Texture* GetTexture();

protected:
	JN_Logging *logObj = NULL;


	///<summary>Checks if two rects are colliding</summary>
	///<param name = "collider">Rect the object is checking against</param>
	bool Collide(SDL_Rect collider);
};

#endif // !JN_GAMEOBJECT_H
