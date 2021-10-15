#pragma once
#include <SDL.h>

struct DrawingInfo
{
	SDL_Rect srcRect;
	SDL_Rect dstRect;
	SDL_Surface* surface;
};