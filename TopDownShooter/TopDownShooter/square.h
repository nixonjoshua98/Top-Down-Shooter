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
	RGB col;
	SDL_Rect rect;

	void virtual Init(int x, int y, RGB _col);
	void virtual Render(SDL_Renderer *renderer);
};


#endif // !SQUARE_H
