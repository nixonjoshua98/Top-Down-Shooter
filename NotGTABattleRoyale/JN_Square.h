#ifndef JN_SQUARE_H
#define JN_SQUARE_H

#include "SDL.h"

struct JN_RGB
{
	int R, G, B;
};

class JN_Square
{
public:

	enum class SquareType {MOVEMENT_DEBUFF, EMPTY, PLAYER, PROJECTILE};

	JN_RGB col;
	SquareType squareType;
	SDL_Rect rect;

	void virtual Init(SquareType type, int x, int y, JN_RGB _col);		// Square init

	void virtual Render(SDL_Renderer *renderer);
	bool Collide(SDL_Rect a, SDL_Rect b);
	float DistanceBetween(SDL_Rect a, SDL_Rect b);
};


#endif // !JN_SQUARE_H
