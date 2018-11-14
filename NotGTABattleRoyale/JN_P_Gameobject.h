#ifndef JN_P_GAMEOBJECT_H
#define JN_P_GAMEOBJECT_H

#include <SDL.h>

#include "JN_P_Sprite.h"

class JN_P_Gameobject
{
public:
	enum class Tag { EMPTY, PLAYER, ENEMY, PLAYER_PROJECTILE, ENEMY_PROJECTILE, MOVEMENT_DEBUFF, DAMAGE };


	///<summary>Default constructor</summary>
	JN_P_Gameobject();

	///<summary>Initilizer</summary>
	///<param name = "tag">Gameobject type</param>
	///<param name = "rect">Objects position</param>
	///<param name = "surface">Sprite surface</param>
	///<param name = "texture">Sprite texture</param>
	void Init(Tag tag, SDL_Rect rect, SDL_Surface *surface, SDL_Texture *texture);


	SDL_Rect rect;
	Tag tag;
	float rotation = 0.0f;

	/// <summary>Collision check between two rects</summary>
	///<param name = "collider">Rect of collider</param>
	bool Collide(SDL_Rect collider);

private:
	JN_P_Sprite sprite;


};


#endif // !JN_P_GAMEOBJECT_H
