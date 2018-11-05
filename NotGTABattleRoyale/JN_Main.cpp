#include "stdafx.h"

#include "JN_GameWorld.h"
#include "JN_Logging.h"


int main(int argc, char *argv[])
{
	JN_ClearLog();

	JN_GameWorld gw = JN_GameWorld();

	if (gw.Init())
		gw.Run();

    return 0;
}