#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "square.h"
#include "SDL.h"
#include "vector2.h"


class Projectile : public Square
{
public:
	SDL_Rect sourceRect = SDL_Rect();
	SDL_Rect targetRect = SDL_Rect();

	bool visible = false;
	Vector2 vector2;

	void Init(RGB _col);
	void Update();
	bool OutOfBounds();
	void Render(SDL_Renderer *renderer) override;
	void LateUpdate();
	void SetTarget(SDL_Rect _sourceRect, SDL_Rect _targetRect);

private:
	static const int PROJECTILE_WIDTH  = 2;
	static const int PROJECTILE_HEIGHT = 7;

	// These are needed as rect only takes ints
	float x = 0;
	float y = 0;
};


#endif // !PROJECTILE_H
