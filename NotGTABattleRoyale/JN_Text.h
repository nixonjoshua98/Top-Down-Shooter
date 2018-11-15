#ifndef JN_TEXT_H
#define JN_TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>


class JN_Text
{
public:
	JN_Text();

	~JN_Text();

	void Init(int x, int y, int w, int h, SDL_Color col, char *fontPath, int size);

	void Render(SDL_Renderer *renderer, std::string txt);

	void ClearText();

	void Move(int xChange, int yChange);

private:
	SDL_Color col;
	TTF_Font *font;
	SDL_Rect rect;
	SDL_Surface *surface = NULL;
	SDL_Texture *texture = NULL;
};


#endif // !JN_TEXT_H
