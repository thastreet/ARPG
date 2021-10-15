#pragma once
#include "AnimationDirection.h"
#include <SDL.h>

const int DIRECTIONS_SIZE = 4;

struct Animation {
	AnimationDirection directions[DIRECTIONS_SIZE];
	SDL_Surface* surface;
};