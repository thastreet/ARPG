#pragma once
#include "Drawing.h"

class Link : public Drawable
{
public:
	void init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader);
	std::vector<SDL_Surface*> getSurfaces();

	vector<DrawingInfo> handleKeyState(const Uint8* keyState, int totalFrame);
};

