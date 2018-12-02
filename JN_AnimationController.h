#ifndef JN_ANIMATIONCONTROLLER_H
#define JN_ANIMATIONCONTROLLER_H

#include "JN_RealTimer.h"
#include "JN_AnimationData.h"

#include <SDL.h>
#include <map>

class JN_AnimationController
{
public:
	// Enum containing the tpes of animation
	enum class Animation {NONE, IDLE, MOVING };
	

	///<summary>Default empty constructor</summary>
	JN_AnimationController();


	///<summary>Deconstructor</summary>
	~JN_AnimationController();


	///<summary>Initilizer</summary>
	void Init(float changeDelay, SDL_Texture *texture);


	///<summary>Add a horizontal spritesheet animtion set</summary>
	void Add(Animation anim, int row, int cellWidth, int cellHeight, int totalCells);


	///<summary>Set the current animation</summary>
	void Set(Animation anim);


	///<summary>Update the animtion</summary>
	void Update();


	///<summary>Render the current frame to scren</summary>
	void Render(SDL_Rect dstRect, float rotation, SDL_Renderer *renderer);

private:
	Animation currentAnimation = Animation::NONE;	// The current animation strip to be used
	int currentAnimIndex = 0;						// The current animation frame
	float changeDelay;								// Delay between switching frames
	float lastSpriteChange;							// The time at the last frame change
	SDL_Texture *texture = NULL;					// Spritesheet
	std::map<Animation, JN_AnimationData> anims;	// Map of the animation
	float lastFrameChange;							// Time at last animation change
};


#endif // !JN_ANIMATIONCONTROLLER_H
