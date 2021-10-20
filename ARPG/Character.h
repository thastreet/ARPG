#pragma once
#include "Drawable.h"
#include <SDL.h>

class Character : public Drawable
{
public:
	virtual SDL_Rect getHitRect() = 0;
	bool intersectsAnyCollision(std::vector<SDL_Rect> collisions);

protected:
	DrawingInfo createDrawingInfo(AnimationDirection animationDirection, Animation* animation);
	int getDirectionIndex();
	void setAnimation(Animation* newAnimation, bool resetFrame);
	void incrementFrame();
	void setDirection(Direction newDirection);
	bool intersectsAnyCollision(std::vector<SDL_Rect> collisions, SDL_Rect* target);

	Animation* animation = nullptr;
	AnimationDirection animationDirection = AnimationDirection();
	Direction direction = Direction::DOWN;
	int x = 0;
	int y = 0;
	int frame = 0;
};

