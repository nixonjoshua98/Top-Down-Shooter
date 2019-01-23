#include "stdafx.h"

#include "JN_AnimationController.h"

#include <iostream>


// Default constructor
JN_AnimationController::JN_AnimationController()
{

}


// Remove memory references
JN_AnimationController::~JN_AnimationController()
{
	texture = NULL;
}


// Init controller
void JN_AnimationController::Init(float changeDelay, SDL_Texture *texture)
{
	this->changeDelay = changeDelay;
	this->texture = texture;
	lastFrameChange = (float)SDL_GetTicks();
	anims = {};
}


// Add a horizontal spritesheet animtion set
void JN_AnimationController::Add(Animation anim, int row, int cellWidth, int cellHeight, int totalCells)
{
	anims[anim] = JN_AnimationData{ row * cellHeight, cellWidth, cellHeight, totalCells };
}


// Set the current aniamtion
void JN_AnimationController::Set(Animation anim)
{
	if (anim != currentAnimation)
	{
		currentAnimation = anim;
		currentAnimIndex = 0;
	}
}


// Updates the current animation
void JN_AnimationController::Update()
{
	float now = (float)SDL_GetTicks();

	if (now >= (lastFrameChange + changeDelay))
	{
		lastFrameChange = now;

		lastSpriteChange = now;
		currentAnimIndex = (currentAnimIndex + 1) % anims[currentAnimation].totalCells;	// Loops over the frames

		if (currentAnimIndex == anims[currentAnimation].totalCells)
			currentAnimIndex = 0;
	}
}


// Render the animation frame
void JN_AnimationController::Render(SDL_Rect dstRect, float rotation, SDL_Renderer *renderer)
{
	if (currentAnimation != Animation::NONE)
	{
		JN_AnimationData animData = anims[currentAnimation];

		SDL_Rect r;
		r.x = currentAnimIndex * animData.cellWidth;
		r.y = animData.yOffset;
		r.w = animData.cellWidth;
		r.h = animData.cellHeight;

		SDL_RenderCopyEx(renderer, texture, &r, &dstRect, rotation, NULL, SDL_FLIP_NONE);
	}
}