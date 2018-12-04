#include "stdafx.h"

#include "JN_GameWorld.h"
#include "JN_Logging.h"
#include "JN_RealTimer.h"

#include <iostream>

/*
	1:	Toggle fullscreen
	2:	Toggle logging
	ESC:	Pause game
*/

/*
	Logging and performance (Row 2):
				PASS:	Log to console
									Performance								COMPLETE
									method times							COMPLETE
									init details							COMPLETE
									input									COMPLETE
									collision								COMPLETE
				2:2:	Seperate render/game-loop/input methods in console	COMPLETE
				2:1:	Game score + health is shown on screen				COMPLETE
				1st:	Log file											COMPLETE

	Window features (Row 3):
				PASS:	Title contains name, module, ID and game name		COMPLETE
				2:2:	Window can be resized and moved						COMPLETE						
				2:1:	Made full screen									COMPLETE
				1st:	Correct aspect ratio maintained						COMPLETE

	Sprites (Row 4):
				PASS:	One sprite on screen								COMPLETE
				2:2:	Multiple moving sprites								COMPLETE
				2:1:	Transparent sprites									COMPLETE
				1st:	Animation											COMPLETE

	Movement, physics, collisions (Row 5):
				PASS:	Players + NPC move around world						COMPLETE
				2:2:	Collision detection between chars					COMPLETE
				2:1:	Cannot move outside of screen, react collison		COMPLETE
				1st:	Game pause											COMPLETE

	Player Character Control (Row 6):
				PASS:	Player can be controlled with 1 button				COMPLETE
				2:2:	Multiple buttons, multiple directions				COMPLETE
				2:1:	Combine actions via keyboard or gamepad				COMPLETE
				1st:	Speed control + gamepad support						NEED GAMEPAD SUPPORT

	Extra features (Row 9):
				PASS:	60 seconds timer									COMPLETE
				2:2:	All main requirments met
				2:1:	Keys can be remapped
				1st:	Saving or loading of game state						COMPLETE
*/


/*
	TODO: Throw away this garbage and start again
*/

int main(int argc, char *argv[])
{
	srand((int)time(NULL));

	JN_GameWorld gw = JN_GameWorld();

	if (gw.Init())
		gw.Run();

    return 0;
}