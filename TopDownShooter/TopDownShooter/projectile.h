#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "square.h"
#include "SDL.h"


class Projectile : public Square
{
public:
	SDL_Rect sourceRect = SDL_Rect();
	SDL_Rect targetRect = SDL_Rect();

	void Init(int x, int y, int destX, int destY);
	void Update();
	bool OutOfBounds();
	void LateUpdate(Square *worldArr);

private:
	bool hitWall = false;
};


#endif // !PROJECTILE_H
