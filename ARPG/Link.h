#pragma once

#include "AnimationLoader.h"
#include "AttackAware.h"
#include "Character.h"
#include "DrawingInfo.h"
#include "SurfaceLoader.h"

class Link : public Character
{
public:
	void init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader);
	std::vector<SDL_Surface*> getSurfaces();
	std::vector<DrawingInfo> tick(const Uint8* keyState, int totalFrame, std::vector<SDL_Rect> collisions);
	SDL_Rect getHitRect();

	AttackAware* attackAware = nullptr;
};

