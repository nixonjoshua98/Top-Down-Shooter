#ifndef JN_ENEMY_H
#define JN_ENEMY_H

#include "JN_AnimationController.h"
#include "JN_WindowData.h"
#include "JN_GameObject.h"
#include "JN_Logging.h"
#include "JN_Player.h"

#include <SDL.h>

class JN_Enemy : public JN_GameObject
{
public:
	JN_Enemy();

	~JN_Enemy();

	bool isDead = false;
	bool isCollidingWithPlayer = false;


	void Init(Tag tag, SDL_Rect rect, SDL_Texture* texture, JN_Logging* logObj, JN_WindowData* windowData);


	void Render(SDL_Renderer* renderer) override;


	void Update(SDL_Rect playerRect);


	void Resize(int x, int y) override;

private:
	JN_Logging* logObj = NULL;

	Tag tag;
	JN_AnimationController animController;

	float lastMoveTime = 0.0f;

	float speedMultiplier = 3.0f;

	float x, y;
};

#endif // !JN_ENEMY_H