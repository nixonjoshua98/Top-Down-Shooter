#include "stdafx.h"

#include "JN_ReadWriteFunctions.h"

#include <string>
#include <fstream>
#include <iostream>

SDL_Surface* LoadBMP(char* path, SDL_Renderer* renderer, bool isTransparent)
{
	SDL_Surface *img = SDL_LoadBMP(path);

	if (isTransparent)
		SDL_SetColorKey(img, 1, SDL_MapRGB(img->format, 255, 255, 255));

	return img;
}



bool ReadTextFile(char* path, char arr[], int length)
{
	std::ifstream f(path);

	if (!f.is_open())
		return false;

	std::string line;
	int i = 0;

	while (std::getline(f, line))
	{
		for (char cell : line)
		{
			if (i + 1 > length)
				break;

			arr[i++] = (char)cell;
		}

		if (i + 1 > length)
			break;
	}

	f.close();

	return true;
}