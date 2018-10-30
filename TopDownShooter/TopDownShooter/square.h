#ifndef SQUARE_H
#define SQUARE_H

#include "SDL.h"

struct RGB
{
	int R;
	int G;
	int B;
};

class Square
{
public:
	static const char EMPTY_SQUARE_CHAR           = '0';
	static const char MOVEMENT_DEBUFF_SQUARE_CHAR = '1';

	enum class SquareType {MOVEMENT_DEBUFF, EMPTY, PLAYER, PROJECTILE};

	RGB col;
	SquareType squareType;
	SDL_Rect rect;

	void virtual Init(SquareType type, int x, int y, RGB _col);
	void virtual Render(SDL_Renderer *renderer);
	bool Collide(SDL_Rect a, SDL_Rect b);
	float DistanceBetween(SDL_Rect a, SDL_Rect b);
};


#endif // !SQUARE_H
