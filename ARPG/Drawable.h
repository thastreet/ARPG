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
	virtual void init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader) = 0;
	virtual std::vector<SDL_Surface*> getSurfaces() = 0;
	virtual std::vector<DrawingInfo> tick(const Uint8* keyState, int totalFrame, std::vector<SDL_Rect> collisions) = 0;

protected:
	DrawingInfo createDrawingInfo(AnimationDirection animationDirection, Animation* animation);
	int getDirectionIndex();
	void setAnimation(Animation* newAnimation, bool resetFrame);
	void incrementFrame();
	void setDirection(Direction newDirection);

	Animation* animation = nullptr;
	AnimationDirection animationDirection = AnimationDirection();
	Direction direction = Direction::DOWN;
	int x = 0;
	int y = 0;
	int frame = 0;
};