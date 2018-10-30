#include "stdafx.h"
#include "gameWorld.h"


int main(int argc, char *argv[])
{
	GameWorld gw = GameWorld();

	if (gw.Init())
	{
		gw.Run();
	}

    return 0;
}

