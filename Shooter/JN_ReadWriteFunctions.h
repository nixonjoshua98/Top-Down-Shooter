#ifndef JN_READWRITEFUNCTIONS_H
#define JN_READWRITEFUNCTIONS_H

#include <SDL.h>
#include <map>

#include "json.hpp"

using json = nlohmann::json;



///<summary>Load bmp file and return surface</summary>
SDL_Surface* LoadBMP(char* path, SDL_Renderer* renderer, bool isTransparent);

bool ReadTextFile(char* path, char arr[], int length);

bool ReadControlFile(char* path);

bool ReadJson(char* path, json &j);

#endif // !JN_READWRITEFUNCTIONS_Haaaaaaa