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
	enum class SquareType {WALL, FLOOR, PLAYER, PROJECTILE};

	RGB col;
	SquareType squareType;
	SDL_Rect rect;

	void virtual Init(SquareType type, int x, int y, RGB _col);
	void virtual Render(SDL_Renderer *renderer);

protected:
	bool Collide(SDL_Rect a, SDL_Rect b);
};


#endif // !SQUARE_H
