#ifndef JN_PROJECTILE_H
#define JN_PROJECTILE_H

#include "SDL.h"

#include "JN_Vector2.h"
#include "JN_Square.h"


class JN_Projectile : public JN_Square
{
public:
	SDL_Rect sourceRect = SDL_Rect();
	SDL_Rect targetRect = SDL_Rect();

	bool visible = false;
	JN_Vector2 vector2;

	void Init(JN_RGB _col);
	void Update();
	bool OutOfBounds();
	void Render(SDL_Renderer *renderer) override;
	void LateUpdate();
	void SetTarget(SDL_Rect _sourceRect, SDL_Rect _targetRect);

private:
	static const int PROJECTILE_WIDTH  = 5;
	static const int PROJECTILE_HEIGHT = 5;

	float speedMultiplier = 15.0f;


	// These are needed as rect only takes ints
	float x = 0;
	float y = 0;
};


#endif // !JN_PROJECTILE_H
