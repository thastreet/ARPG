#pragma once
#include "Drawable.h"
#include <SDL.h>

class Character : public Drawable
{
public:
	virtual SDL_Rect getHitRect() = 0;
};

