#ifndef JN_READWRITEFUNCTIONS_H
#define JN_READWRITEFUNCTIONS_H

#include <SDL.h>


///<summary>Load bmp file and return surface</summary>
SDL_Surface* LoadBMP(char* path, SDL_Renderer* renderer, bool isTransparent);


bool ReadTextFile(char* path, char arr[], int length);

#endif // !JN_READWRITEFUNCTIONS_H
