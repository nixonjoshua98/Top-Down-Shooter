#include "stdafx.h"

#include "JN_GameWorld.h"
#include "JN_Logging.h"
#include "JN_RealTimer.h"
#include "JN_SplashScreen.h"
#include "JN_Menu.h"

#include <iostream>

/*
	Logging and performance (Row 2):
				PASS:	Log to console										COMPLETE
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
				1st:	Speed control + gamepad support						COMPLETE

	Menu (Row 7):
				PASS:	Splash screen (contains title and controls)			COMPLETE
				2:2:	Menu, elements can be accessed using the keyboard	COMPLETE
				2:1:	Menu elements highlight in colour when selected		COMPLETE
				1st:	menu controller with mouse + sub-menus				COMPLETE

	Audio (Row 8):
				PASS:	Clear background audio								COMPLETE
				2:2:	Sound effects										COMPLETE
				2:1:	Audio volume controlled in-game						COMPLETE
				1st:	Mute/un-mute SFX/BGM								COMPLETE

	Extra features (Row 9):
				PASS:	60 seconds timer									COMPLETE
				2:2:	All main requirments met							COMPLETE
				2:1:	Keys can be remapped								COMPLETE
				1st:	Saving or loading of game state						COMPLETE
*/


int main(int argc, char *argv[])
{
	srand((int)time(NULL));

	JN_GameWorld* gw = new JN_GameWorld();
	JN_SplashScreen ss = JN_SplashScreen();
	JN_Menu menu = JN_Menu();

	if (gw->Init())
	{
		ss.Run(gw->GetRenderer(), gw->GetTiles(), 5);

		if (!ss.GetQuit())
		{
			menu.Init();

			menu.Run(gw->GetRenderer(), gw->GetTiles(), gw, gw->GetLog());

			if (!menu.GetQuit())
				gw->Run();
		}
	}

	delete gw;
	gw = NULL;

    return 0;
}