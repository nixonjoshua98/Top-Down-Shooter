#include "stdafx.h"

#include "JN_GameWorld.h"
#include "JN_Logging.h"

/*
	Resizing window to max causes a 'missing tile'	| Fixed (hacky solution)
	Some scancodes are be accepted using other scancodes	| Ask 3rd party for new eyes
	tiles vector pointers deletion question	| Vectors pointers in 2 dif vectors, delete 1 or 2?
	resizaing is broke (Y)	| Fixed (kinda)
	Reize to maximum has broken	| It suddenly started working again? dafuk
	Idea: Pass playern  projectiles which need the out of bounds check a struct reference to the windows height etc.
*/


int main(int argc, char *argv[])
{
	JN_ClearLog();

	JN_GameWorld gw = JN_GameWorld();

	if (gw.Init())
		gw.Run();

    return 0;
}