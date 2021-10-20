#pragma once
#include "Animation.h"
#include "AnimationDirection.h"
#include "AnimationLoader.h"
#include "Direction.h"
#include "DrawingInfo.h"
#include "SurfaceLoader.h"
#include <vector>

class Drawable
{
public:
	virtual void init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader, int screenWidth, int screenHeight) = 0;
	virtual std::vector<SDL_Surface*> getSurfaces() = 0;
	virtual std::vector<DrawingInfo> tick(const Uint8* keyState, int totalFrame, std::vector<SDL_Rect> collisions) = 0;
};