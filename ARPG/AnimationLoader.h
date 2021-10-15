#pragma once
#include "Animation.h"
#include <string>

class AnimationLoader
{
public:
	void loadAnimation(Animation* animation, std::string name, std::string key, SDL_Surface* surface);
};