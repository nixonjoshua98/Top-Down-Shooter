#ifndef JN_ASSET_H
#define JN_ASSET_H

#include "JN_ReadWriteFunctions.h"
#include "JN_Logging.h"

#include <SDL.h>


class JN_Asset
{
public:
	void Init(SDL_Renderer* renderer, char* path, bool isTransparent, JN_Logging* logObj)
	{
		this->logObj = logObj;
		this->path = path;

		surface = LoadBMP(path, renderer, isTransparent);
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		if (texture != NULL)
			logObj->LogMethod(std::string(path) + " has been read in");
		else
			logObj->LogMethod(std::string(path) + " has failed to be read in");
	}

	void Cleanup()
	{
		if (surface != NULL)
		{
			SDL_FreeSurface(surface);
			surface = NULL;
			logObj->LogMethod(std::string(path) + " surface has been freed");
		}

		if (texture != NULL)
		{
			SDL_DestroyTexture(texture);
			texture = NULL;
			logObj->LogMethod(std::string(path) + " texture has been destroyed");
		}
	}

	SDL_Texture* GetTexture() {
		return texture;
	}

private:
	char* path;

	JN_Logging *logObj = NULL;
	SDL_Surface* surface = NULL;
	SDL_Texture* texture = NULL;
};

#endif // !JN_ASSET_H
