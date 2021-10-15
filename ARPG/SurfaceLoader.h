#pragma once
#include <SDL.h>
#include <string>

class SurfaceLoader
{
public:
	SDL_Surface* getWindowSurface(SDL_Window* window);
	SDL_Surface* loadSurface(std::string path, SDL_Surface* windowSurface);
};