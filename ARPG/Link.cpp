#include "Link.h"

const bool betterSword = false;
const int walkDistance = 2;
const int walkingAnimationThreshold = 3;

bool attacking = false;

Animation walkingAnimation = Animation();
Animation stoppedAnimation = Animation();
Animation attackingAnimation = Animation();
Animation swordAnimation = Animation();

void Link::init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader)
{
	animationLoader->loadAnimation(&walkingAnimation, "tunic1.json", "walking", surfaceLoader->loadSurface("walking.tunic.png", windowSurface));
	animationLoader->loadAnimation(&stoppedAnimation, "tunic1.json", "stopped", surfaceLoader->loadSurface("stopped.tunic.png", windowSurface));
	animationLoader->loadAnimation(&attackingAnimation, "tunic1.json", "sword", surfaceLoader->loadSurface("sword.tunic1.png", windowSurface));
	animationLoader->loadAnimation(&swordAnimation, "sword1.json", "sword", surfaceLoader->loadSurface(betterSword ? "sword.sword2.png" : "sword.sword1.png", windowSurface));

	setAnimation(&stoppedAnimation, true);

	x = animationDirection.offsetX;
	y = animationDirection.offsetY;
}

std::vector<SDL_Surface*> Link::getSurfaces()
{
	std::vector<SDL_Surface*> surfaces;
	surfaces.push_back(walkingAnimation.surface);
	surfaces.push_back(stoppedAnimation.surface);
	surfaces.push_back(attackingAnimation.surface);
	surfaces.push_back(swordAnimation.surface);
	return surfaces;
}

bool intersectsAnyCollision(std::vector<SDL_Rect> collisions, SDL_Rect* target)
{
	bool intersectAnyRect = false;
	for (auto rect : collisions)
	{
		if (SDL_HasIntersection(&rect, target))
		{
			return true;
		}
	}

	return false;
}

bool Link::onLeftPressed(const Uint8* keyState, std::vector<SDL_Rect> collisions, bool frameAlreadyIncremented, bool shouldAnimateWalking)
{
	bool frameIncremented = false;

	if (!keyState[SDL_SCANCODE_DOWN] && !keyState[SDL_SCANCODE_UP] || direction == Direction::RIGHT)
	{
		setDirection(Direction::LEFT);
	}

	if (shouldAnimateWalking)
	{
		if (!frameAlreadyIncremented)
		{
			incrementFrame();
			frameIncremented = true;
		}

		SDL_Rect hitRect = getHitRect();
		hitRect.x -= walkDistance;

		if (!intersectsAnyCollision(collisions, &hitRect))
		{
			x -= walkDistance;
		}
	}

	return frameIncremented;
}

bool Link::onRightPressed(const Uint8* keyState, std::vector<SDL_Rect> collisions, bool frameAlreadyIncremented, bool shouldAnimateWalking)
{
	bool frameIncremented = false;

	if (!keyState[SDL_SCANCODE_DOWN] && !keyState[SDL_SCANCODE_UP] || direction == Direction::LEFT)
	{
		setDirection(Direction::RIGHT);
	}

	if (shouldAnimateWalking)
	{
		if (!frameAlreadyIncremented)
		{
			incrementFrame();
			frameIncremented = true;
		}

		SDL_Rect hitRect = getHitRect();
		hitRect.x += walkDistance;

		if (!intersectsAnyCollision(collisions, &hitRect))
		{
			x += walkDistance;
		}
	}

	return frameIncremented;
}

bool Link::onUpPressed(const Uint8* keyState, std::vector<SDL_Rect> collisions, bool frameAlreadyIncremented, bool shouldAnimateWalking)
{
	bool frameIncremented = false;

	if (!keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT] || direction == Direction::DOWN)
	{
		setDirection(Direction::UP);
	}

	if (shouldAnimateWalking)
	{
		if (!frameAlreadyIncremented)
		{
			incrementFrame();
			frameIncremented = true;
		}

		SDL_Rect hitRect = getHitRect();
		hitRect.y -= walkDistance;

		if (!intersectsAnyCollision(collisions, &hitRect))
		{
			y -= walkDistance;
		}
	}

	return frameIncremented;
}

bool Link::onDownPressed(const Uint8* keyState, std::vector<SDL_Rect> collisions, bool frameAlreadyIncremented, bool shouldAnimateWalking)
{
	bool frameIncremented = false;

	if (!keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT] || direction == Direction::UP)
	{
		setDirection(Direction::DOWN);
	}

	if (shouldAnimateWalking)
	{
		if (!frameAlreadyIncremented)
		{
			incrementFrame();
			frameIncremented = true;
		}

		SDL_Rect hitRect = getHitRect();
		++hitRect.y;

		if (!intersectsAnyCollision(collisions, &hitRect))
		{
			y += walkDistance;
		}
	}

	return frameIncremented;
}

void Link::continueAttacking()
{
	incrementFrame();
	if (frame == animationDirection.size / 2)
	{
		int swordX = 0;
		int swordY = 0;

		switch (direction)
		{
		case Direction::DOWN:
			swordX = animationDirection.width / 2;
			swordY = animationDirection.height + 6;
			break;
		case Direction::UP:
			swordX = animationDirection.width / 2;
			swordY = 0;
			break;
		case Direction::LEFT:
			swordX = -6;
			swordY = animationDirection.height - 10;
			break;
		case Direction::RIGHT:
			swordX = animationDirection.width + 6;
			swordY = animationDirection.height - 10;
			break;
		}

		attackAware->attack(x - animationDirection.offsetX + swordX, y - animationDirection.offsetY + swordY);
	}
}

void Link::handleKeyState(const Uint8* keyState, bool shouldAnimateWalking, std::vector<SDL_Rect> collisions)
{
	bool frameIncremented = false;

	if (keyState[SDL_SCANCODE_A])
	{
		attacking = true;
		setAnimation(&attackingAnimation, true);
	}
	else if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_DOWN] || keyState[SDL_SCANCODE_UP])
	{
		setAnimation(&walkingAnimation, false);
	}
	else
	{
		setAnimation(&stoppedAnimation, true);
	}

	if (!attacking)
	{
		if (keyState[SDL_SCANCODE_RIGHT])
		{
			frameIncremented = onRightPressed(keyState, collisions, frameIncremented, shouldAnimateWalking);
		}
		if (keyState[SDL_SCANCODE_LEFT])
		{
			frameIncremented = onLeftPressed(keyState, collisions, frameIncremented, shouldAnimateWalking);
		}
		if (keyState[SDL_SCANCODE_UP])
		{
			frameIncremented = onUpPressed(keyState, collisions, frameIncremented, shouldAnimateWalking);
		}
		if (keyState[SDL_SCANCODE_DOWN])
		{
			frameIncremented = onDownPressed(keyState, collisions, frameIncremented, shouldAnimateWalking);
		}
	}
}

void Link::finishAttackingIfNecessary()
{
	if (attacking && frame == animationDirection.size - 1)
	{
		attacking = false;
		frame = 0;
	}
}

std::vector<DrawingInfo> Link::tick(const Uint8 * keyState, int totalFrame, std::vector<SDL_Rect> collisions, int screenWidth, int screenHeight)
{
	const bool shouldAnimateWalking = totalFrame % walkingAnimationThreshold == 0;

	finishAttackingIfNecessary();

	if (attacking)
	{
		continueAttacking();
	}
	else
	{
		handleKeyState(keyState, shouldAnimateWalking, collisions);
	}

	std::vector<DrawingInfo> drawingInfos;
	drawingInfos.push_back(createDrawingInfo(animationDirection, animation));

	if (attacking)
	{
		drawingInfos.push_back(createDrawingInfo(swordAnimation.directions[getDirectionIndex()], &swordAnimation));
	}

	return drawingInfos;
}

SDL_Rect Link::getHitRect()
{
	const int horizontalSpace = 5;
	const int hitHeight = 12;

	SDL_Rect rect = SDL_Rect();
	rect.x = x - animationDirection.offsetX + horizontalSpace;
	rect.y = y + animationDirection.height - animationDirection.offsetY - hitHeight;
	rect.w = animationDirection.width - 2 * horizontalSpace;
	rect.h = hitHeight;

	return rect;
}