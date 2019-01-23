#ifndef JN_SQUARE_H
#define JN_SQUARE_H

#include <SDL.h>
#include <iostream>

class JN_Square
{

public:
	int R, G, B;
	SDL_Rect rect;

	void Init(int x, int y, int w, int h, int r, int g, int b)
	{
		R = r;
		G = g;
		B = b;

		this->rect = SDL_Rect{ x, y, w, h };
	}

	void Render(SDL_Renderer* renderer, bool fill)
	{
		SDL_SetRenderDrawColor(renderer, R, G, B, 255);

		if (fill)
			SDL_RenderFillRect(renderer, &rect);
		else
			SDL_RenderDrawRect(renderer, &rect);
	}
};

#endif // !JN_SQUARE_H
