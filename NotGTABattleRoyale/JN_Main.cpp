#include "stdafx.h"

#include "JN_GameWorld.h"
#include "JN_Logging.h"
#include "JN_RealTimer.h"

#include <iostream>

/*
	Logging and performance (Row 2):
				PASS:	Log to console (Performance, method times, init details, input, collision)
				2:2:	Seperate render/game-loop/input functions in console
				2:1:	Game score is shown on screen
				1st:	Log file

	Window features (Row 3):
				PASS:	Title contains name, module, ID and game name		COMPLETE
				2:2:	Window can be resized and moved						COMPLETE						
				2:1:	Made full screen									
				1st:	Correct aspect ratio maintained						COMPLETE (???)

	Sprites (Row 4):
				PASS:	One sprite on screen								COMPLETE
				2:2:	Multiple moving sprites
				2:1:	Transparent sprites									COMPLETE
				1st:	Animation

	Player Character Control (Row 6):
				PASS:	Player can be controlled with 1 button				COMPLETE
				2:2:	Multiple buttons, multiple directions				COMPLETE
				2:1:	Combine actions via keyboard or gamepad				COMPLETE
				1st:	Speed control + gamepad support
*/

int main(int argc, char *argv[])
{
	JN_GameWorld gw = JN_GameWorld();

	if (gw.Init())
		gw.Run();

    return 0;
}