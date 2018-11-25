#ifndef JN_PROJECTILE_H
#define JN_PROJECTILE_H

#include "SDL.h"

#include "JN_Logging.h"
#include "JN_Vector2.h"
#include "JN_WindowData.h"
#include "JN_Gameobject.h"


class JN_Projectile : public JN_Gameobject
{
public:
	~JN_Projectile();

	SDL_Rect sourceRect = SDL_Rect();
	SDL_Rect targetRect = SDL_Rect();

	bool visible = false;
	JN_Vector2 vector2;

	void Init(JN_Gameobject::Tag tag, JN_Logging *logObj, JN_WindowData *windowData);
	void Update();
	bool OutOfBounds();
	void Render(SDL_Renderer *renderer);
	void LateUpdate();
	void SetTarget(SDL_Rect sourceRect, SDL_Rect targetRect);

private:
	static const int PROJECTILE_WIDTH  = 25;
	static const int PROJECTILE_HEIGHT = 25;

	float speedMultiplier = 15.0f;

	JN_Logging *logObj = NULL;
	JN_WindowData *windowData = NULL;


	// These are needed as rect only takes ints
	float x = 0;
	float y = 0;
};


#endif // !JN_PROJECTILE_H
