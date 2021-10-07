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

DrawingInfo Drawable::createDrawingInfo(AnimationDirection animationDirection, Animation* animation)
{
	SDL_Rect src = SDL_Rect();
	src.x = animationDirection.x + animationDirection.width * frame;
	src.y = animationDirection.y;
	src.w = animationDirection.width;
	src.h = animationDirection.height;

	SDL_Rect dst = SDL_Rect();
	dst.x = x - animationDirection.offsetX;
	dst.y = y - animationDirection.offsetY;
	dst.w = animationDirection.width;
	dst.h = animationDirection.height;

	DrawingInfo drawingInfo = DrawingInfo();
	drawingInfo.srcRect = src;
	drawingInfo.dstRect = dst;
	drawingInfo.surface = animation->surface;

	return drawingInfo;
}

int Drawable::getDirectionIndex()
{
	switch (direction)
	{
	case Direction::DOWN:
		return 3;
	case Direction::UP:
		return 1;
	case Direction::LEFT:
		return 2;
	case Direction::RIGHT:
		return 0;
	default:
		return 0;
	}
}

void Drawable::setAnimation(Animation* newAnimation, bool resetFrame)
{
	animation = newAnimation;
	animationDirection = animation->directions[getDirectionIndex()];
	if (resetFrame)
	{
		frame = 0;
	}
}

void Drawable::incrementFrame()
{
	++frame;
	if (frame > animationDirection.size - 1)
	{
		frame = 0;
	}
}

void Drawable::setDirection(Direction newDirection)
{
	direction = newDirection;
	animationDirection = animation->directions[getDirectionIndex()];
}
