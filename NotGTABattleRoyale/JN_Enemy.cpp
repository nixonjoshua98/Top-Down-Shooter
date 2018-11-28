#include "stdafx.h"

#include "JN_Vector2.h"
#include "JN_Enemy.h"

JN_Enemy::JN_Enemy()
{

}


JN_Enemy::~JN_Enemy()
{
	logObj = NULL;
}


void JN_Enemy::Init(Tag tag, SDL_Rect rect, SDL_Texture* texture, JN_Logging* logObj, JN_WindowData* windowData)
{
	JN_GameObject::Init(tag, texture, rect, logObj);

	this->logObj = logObj;
	this->tag = tag;

	x = rect.x;
	y = rect.y;

	animController.Init(200, texture);

	switch (tag)
	{
	case Tag::SCORPION:
		this->rect = SDL_Rect{ rect.x, rect.w, 32, 32 };
		animController.Add(JN_AnimationController::Animation::MOVING, 0, 64, 64, 3);
		break;
	}

	animController.Set(JN_AnimationController::Animation::MOVING);
}


void JN_Enemy::Render(SDL_Renderer* renderer)
{
	animController.Render(rect, rotationAngle, renderer);
}


void JN_Enemy::Update(JN_Player *player)
{
	animController.Update();

	float now = (float)SDL_GetTicks();

	if (now - lastMoveTime >= 100)
	{
		lastMoveTime = now;

		JN_Vector2 v = JN_Vector2(player->rect.x, player->rect.y).Normalise();

		x += v.x * 1.5f;
		y += v.y * 1.5f;

		/*if (player->rect.x > rect.x)
			x += v.x * 1.5f;
		else
			x -= v.x * 1.5f;

		if (player->rect.y > rect.y)
			y += v.y * 1.5f;
		else
			y -= v.y * 1.5f;
		*/

		rect.x = x;
		rect.y = y;

		rotationAngle = (float)(atan2(player->rect.y - rect.y, player->rect.x - rect.x) * 180.0f / 3.14159);
	}
}

void JN_Enemy::Resize(int x, int y)
{
	this->x += x;
	this->y += y;
}