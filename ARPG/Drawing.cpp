#include "Drawing.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "json.hpp"
#include <iostream>
#include <fstream>

using namespace nlohmann;

SDL_Surface* SurfaceLoader::getWindowSurface(SDL_Window* window)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return NULL;
	}
	else
	{
		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return NULL;
		}
		else
		{
			if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
			{
				printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				return NULL;
			}
			else
			{
				return SDL_GetWindowSurface(window);
			}
		}
	}

	return NULL;
}

SDL_Surface* SurfaceLoader::loadSurface(std::string path, SDL_Surface* windowSurface)
{
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, windowSurface->format, 0);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}

AnimationDirection createAnimationDirection(json direction)
{
	AnimationDirection animationDirection = AnimationDirection();
	animationDirection.x = direction["x"];
	animationDirection.y = direction["y"];
	animationDirection.width = direction["frame_width"];
	animationDirection.height = direction["frame_height"];
	animationDirection.offsetX = direction["origin_x"];
	animationDirection.offsetY = direction["origin_y"];
	if (direction.contains("num_frames")) {
		animationDirection.size = direction["num_frames"];
	}
	else {
		animationDirection.size = 1;
	}
	return animationDirection;
}

void AnimationLoader::loadAnimation(Animation* animation, string name, string key, SDL_Surface* surface)
{
	animation->surface = surface;

	ifstream i(name);
	json j;
	i >> j;

	for (auto& element : j) {
		if (element["name"] == key) {
			json directions = element["directions"];
			for (int i = 0; i < DIRECTIONS_SIZE; ++i) {
				animation->directions[i] = createAnimationDirection(directions[i]);
			}
			break;
		}
	}
}