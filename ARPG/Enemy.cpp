#include "Enemy.h"
#include <random>

const int animationThreshold = 20;
const int moveThreshold = 40;
Animation mainAnimation = Animation();

std::mt19937 seed;
std::uniform_int_distribution<std::mt19937::result_type> moveRng;
std::uniform_int_distribution<std::mt19937::result_type> stepRng;

void Enemy::init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader, int screenWidth, int screenHeight)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	seed = rng;
	
	std::uniform_int_distribution<std::mt19937::result_type> moveFunction(0, 2);
	std::uniform_int_distribution<std::mt19937::result_type> stepFunction(0, 4);
	moveRng = moveFunction;
	stepRng = stepFunction;

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

std::vector<DrawingInfo> Enemy::tick(const Uint8* keyState, int totalFrame, std::vector<SDL_Rect> collisions)
{
	const bool shouldAnimate = totalFrame % animationThreshold == 0;
	if (shouldAnimate)
	{
		incrementFrame();
	}

	const bool shouldMove = totalFrame % moveThreshold == 0;
	if (shouldMove)
	{
		int randomX = moveRng(seed);
		int randomY = moveRng(seed);
		int step = stepRng(seed);

		if (randomX == 0) {
			x += -step;
		}
		else if (randomX == 1)
		{
			x += step;
		}

		if (randomY == 0) {
			y += -step;
		}
		else if (randomY == 1)
		{
			y += step;
		}
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