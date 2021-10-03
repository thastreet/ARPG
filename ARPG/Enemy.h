#pragma once
#include "Drawing.h"

class Enemy : public Drawable
{
public:
	void init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader);
	vector<SDL_Surface*> getSurfaces();
	vector<DrawingInfo> tick(const Uint8* keyState, int totalFrame);
	SDL_Rect getHitRect();
};

