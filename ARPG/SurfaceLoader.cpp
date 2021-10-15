#include "SurfaceLoader.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

SDL_Surface* SurfaceLoader::getWindowSurface(SDL_Window* window)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return nullptr;
	}
	else
	{
		if (window == nullptr)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return nullptr;
		}
		else
		{
			if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				return nullptr;
			}
			else
			{
				return SDL_GetWindowSurface(window);
			}
		}
	}

	return nullptr;
}

SDL_Surface* SurfaceLoader::loadSurface(std::string path, SDL_Surface* windowSurface)
{
	SDL_Surface* optimizedSurface = nullptr;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == nullptr)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		optimizedSurface = SDL_ConvertSurface(loadedSurface, windowSurface->format, 0);
		if (optimizedSurface == nullptr)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}