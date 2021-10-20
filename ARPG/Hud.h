#pragma once
#include "Drawable.h"

class Hud : public Drawable
{
public:
	void init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader, int screenWidth, int screenHeight);
	std::vector<SDL_Surface*> getSurfaces();
	std::vector<DrawingInfo> tick(const Uint8* keyState, int totalFrame, std::vector<SDL_Rect> collisions);

	void update(int life);
};

