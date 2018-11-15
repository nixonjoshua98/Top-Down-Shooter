#ifndef JN_PLAYER_H
#define JN_PLAYER_H

#include "SDL.h"

#include "JN_Gameobject.h"
#include "JN_RealTimer.h"
#include "JN_ProjectileController.h"
#include "JN_HealthController.h"
#include "JN_PlayerControls.h"
#include "JN_WindowData.h"

#include <set>
#include <map>

class JN_Player : public JN_Gameobject
{
public:
	///<summary>Default constructor</summary>
	JN_Player();


	///<summary>Deconstructor</summary>
	~JN_Player();


	///<summary>Initialises the player objects</summary>
	///<param name = "renderer">Used to load the images</param>
	///<param name = "logObj">Logging object used to log events to file and console</param>
	///<param name = "windowSize">Pointer to the windows width and height</param>
	void Init(SDL_Renderer *renderer, JN_Logging *logObj, JN_WindowData *windowData);


	///<summary>Method which handles the user input</summary>
	///<param name = "e">The event object</param>
	void Input(SDL_Event e);


	///<summary>Method which is called every frame</summary>
	void Update();


	///<summary>Called after Update and handles collisions</summary>
	///<param name = "tiles">The tiles which the player could potentially collide with</param>
	void LateUpdate(std::vector<JN_Gameobject*> tiles);


	///<summary>Renders the player to the screen</summary>
	///<param name = "renderer">The renderer which the player will be on</param>
	void Render(SDL_Renderer *renderer) override;


	///<summary>Drops all input so the player has a fresh start</summary>
	void EmptyInput();

private:
	const int MOVEMENT_SPEED = 3;	// The amount the player moves each movement cycle
	const int MOVEMENT_DELAY = 25;	// The delay between movements
	const int SHOOT_DELAY = 250;	// The delay between being able to fire a projectile
	const int PLAYER_WIDTH = 20;	// ...
	const int PLAYER_HEIGHT = 20;	// ...


	const int DAMAGE_TILE_AMOUNT = 1;			// Damage every 0.25s while the user touches the collider
	const float MOVEMENT_DEBUFF_AMOUNT = 0.5f;	// Movement will be multiplied by this


	JN_Logging *logObj = NULL;							// Log object
	JN_WindowData *windowData = NULL;					// Window data, stores size and offsets
	JN_HealthController health;							// Player health controller with a starting value of 100
	JN_RealTimer damageTileTimer;						// Timer for damage tile delays
	JN_ProjectileController projectileController;		// Creates the controller with a maximum of 10 projectiles on screen at once
	JN_PlayerControls controls;							// The controls object for the player, deals with all input


	// Player buffs and debuffs
	std::map<Tag, bool> statusEffects = {
		{ Tag::MOVEMENT_DEBUFF, false }
	};


	SDL_Rect newRect = SDL_Rect();	// Temp rect used during collision detection


	float lastMovementTime = 0;	// Movement delay timer
	float lastShootTime    = 0;	// Shoot timer
	float lastDmgFromTile  = 0;	


	///<summary>Moves the player based on the input</summary>
	void Move();


	///<summary>Attempts to shoot a projectile</summary>
	void Shoot();


	///<summary>Rotates the player to face the cursor</summary>
	void RotatePlayer();


	///<summary>Handles the keyboard input</summary>
	///<param name = "e">The event object</param>
	void KeyboardInputHandler(SDL_Event e);


	///<summary>Method to update the animation</summary>
	void AnimationUpdate();


	///<summary>Handles the mouse input</summary>
	///<param name = "e">The event object</param>
	void MouseInputHandler(SDL_Event e);


	///<summary>Sets the player position after collision checking</summary>
	void ConfirmPlayerMovement();


	///<summary>Returns a set containing the game object types</summary>
	///<param name = "tiles">vector of potential collisions</param>
	std::set<Tag> GetColliders(std::vector<JN_Gameobject*> tiles);


	///<summary>Actions based on collisions</summary>
	///<param name = "tiles">vector of potential collisions</param>
	void ColliderManager(std::vector<JN_Gameobject*> tiles);


	///<summary>Resets all player status effects</summary>
	void ResetBuffs();
};

#endif // !PLAYER_H
