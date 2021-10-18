#include "Enemy.h"

const int animationThreshold = 20;
Animation mainAnimation = Animation();

void Enemy::init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader)
{
	animationLoader->loadAnimation(&mainAnimation, "tentacle.json", "walking", surfaceLoader->loadSurface("tentacle.png", windowSurface));

	setAnimation(&mainAnimation, true);

	x = animationDirection.offsetX + 100;
	y = animationDirection.offsetY + 50;
}

std::vector<SDL_Surface*> Enemy::getSurfaces()
{
	std::vector<SDL_Surface*> surfaces;
	surfaces.push_back(mainAnimation.surface);
	return surfaces;
}

std::vector<DrawingInfo> Enemy::tick(const Uint8* keyState, int totalFrame, std::vector<SDL_Rect> collisions, int screenWidth, int screenHeight)
{
	const bool shouldAnimate = totalFrame % animationThreshold == 0;
	if (shouldAnimate)
	{
		incrementFrame();
	}

	std::vector<DrawingInfo> drawingInfos;
	drawingInfos.push_back(createDrawingInfo(animationDirection, &mainAnimation));
	return drawingInfos;
}

SDL_Rect Enemy::getHitRect()
{
	const int hitHeight = 14;

	SDL_Rect rect = SDL_Rect();
	rect.x = x - animationDirection.offsetX;
	rect.y = y + animationDirection.height - animationDirection.offsetY - hitHeight;
	rect.w = animationDirection.width;
	rect.h = hitHeight;

	return rect;
}

void Enemy::attack(int value)
{
	int newLife = life - value;
	if (newLife < 0)
	{
		life = 0;
	}
	else
	{
		life = newLife;
	}
}