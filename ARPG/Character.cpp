#include "Character.h"

DrawingInfo Character::createDrawingInfo(AnimationDirection animationDirection, Animation* animation)
{
	SDL_Rect src = SDL_Rect();
	src.x = animationDirection.x + animationDirection.width * frame;
	src.y = animationDirection.y;
	src.w = animationDirection.width;
	src.h = animationDirection.height;

	SDL_Rect dst = SDL_Rect();
	dst.x = x - animationDirection.offsetX;
	dst.y = y - animationDirection.offsetY;
	dst.w = animationDirection.width;
	dst.h = animationDirection.height;

	DrawingInfo drawingInfo = DrawingInfo();
	drawingInfo.srcRect = src;
	drawingInfo.dstRect = dst;
	drawingInfo.surface = animation->surface;

	return drawingInfo;
}

int Character::getDirectionIndex()
{
	switch (direction)
	{
	case Direction::DOWN:
		return 3;
	case Direction::UP:
		return 1;
	case Direction::LEFT:
		return 2;
	case Direction::RIGHT:
		return 0;
	default:
		return 0;
	}
}

void Character::setAnimation(Animation* newAnimation, bool resetFrame)
{
	animation = newAnimation;
	animationDirection = animation->directions[getDirectionIndex()];
	if (resetFrame)
	{
		frame = 0;
	}
}

void Character::incrementFrame()
{
	++frame;
	if (frame > animationDirection.size - 1)
	{
		frame = 0;
	}
}

void Character::setDirection(Direction newDirection)
{
	direction = newDirection;
	animationDirection = animation->directions[getDirectionIndex()];
}
