
#include "stdafx.h"
#include "JN_Logging.h"

#include <iostream>
#include <fstream>

void JN_Log(std::string obj, std::string desc, bool consoleOnly)
{
	return;

	std::string txt = obj + " | " + desc;

	std::cout << "\n" << txt;

	if (consoleOnly)
		return;

	std::ofstream f;

	f.open("Data/Log.log", std::ios_base::app);

	if (f.is_open())
	{
		f << txt << "\n";
		f.close();
	}
}

void JN_ClearLog()
{
	std::ofstream f;

	f.open("Data/Log.log", std::ios_base::trunc | std::ios_base::out);

	if (f.is_open())
	{
		f.close();
	}
}