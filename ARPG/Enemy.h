#pragma once
#include "Character.h"

class Enemy : public Character
{
public:
	void init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader, int screenWidth, int screenHeight);
	std::vector<SDL_Surface*> getSurfaces();
	std::vector<DrawingInfo> tick(const Uint8* keyState, int totalFrame, std::vector<SDL_Rect> collisions);
	void attack(int value);
	SDL_Rect getHitRect();

	int life = 3;
};

