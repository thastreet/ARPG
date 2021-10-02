#include "Link.h"

const bool betterSword = false;
const int walkDistance = 2;
const int walkingAnimationThreshold = 3;
const int attackingAnimationThreshold = 2;
int x;
int y;
int frame = 0;
bool attacking = false;

Animation walkingAnimation = Animation();
Animation stoppedAnimation = Animation();
Animation attackingAnimation = Animation();
Animation swordAnimation = Animation();

Animation* animation;
AnimationDirection animationDirection;

Direction direction = Direction::DOWN;

int getDirectionIndex()
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
	}
}

void setAnimation(Animation* newAnimation, bool resetFrame)
{
	animation = newAnimation;
	animationDirection = animation->directions[getDirectionIndex()];
	if (resetFrame)
	{
		frame = 0;
	}
}

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
	std::vector<SDL_Surface*> vect;
	vect.push_back(walkingAnimation.surface);
	vect.push_back(stoppedAnimation.surface);
	vect.push_back(attackingAnimation.surface);
	vect.push_back(swordAnimation.surface);
	return vect;
}

void incrementFrame()
{
	++frame;
	if (frame > animationDirection.size - 1)
	{
		frame = 0;
	}
}

void setDirection(Direction newDirection)
{
	direction = newDirection;
	animationDirection = animation->directions[getDirectionIndex()];
}

DrawingInfo createDrawingInfo(AnimationDirection animationDirection, Animation* animation)
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

vector<DrawingInfo> Link::handleKeyState(const Uint8 * keyState, int totalFrame)
{
	const bool shouldAnimateWalking = totalFrame % walkingAnimationThreshold == 0;
	const bool shouldAnimateAttacking = totalFrame % attackingAnimationThreshold == 0;

	if (attacking && frame == animationDirection.size - 1)
	{
		attacking = false;
		frame = 0;
	}

	if (attacking)
	{
		if (shouldAnimateAttacking)
		{
			incrementFrame();
		}
	}
	else
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
				if (!keyState[SDL_SCANCODE_DOWN] && !keyState[SDL_SCANCODE_UP] || direction == Direction::LEFT)
				{
					setDirection(Direction::RIGHT);
				}

				if (shouldAnimateWalking)
				{
					if (!frameIncremented)
					{
						incrementFrame();
						frameIncremented = true;
					}
					x += walkDistance;
				}
			}
			if (keyState[SDL_SCANCODE_LEFT])
			{
				if (!keyState[SDL_SCANCODE_DOWN] && !keyState[SDL_SCANCODE_UP] || direction == Direction::RIGHT)
				{
					setDirection(Direction::LEFT);
				}

				if (shouldAnimateWalking)
				{
					if (!frameIncremented)
					{
						incrementFrame();
						frameIncremented = true;
					}
					x -= walkDistance;
				}
			}
			if (keyState[SDL_SCANCODE_UP])
			{
				if (!keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT] || direction == Direction::DOWN)
				{
					setDirection(Direction::UP);
				}

				if (shouldAnimateWalking)
				{
					if (!frameIncremented)
					{
						incrementFrame();
						frameIncremented = true;
					}
					y -= walkDistance;
				}
			}
			if (keyState[SDL_SCANCODE_DOWN])
			{
				if (!keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT] || direction == Direction::UP)
				{
					setDirection(Direction::DOWN);
				}

				if (shouldAnimateWalking)
				{
					if (!frameIncremented)
					{
						incrementFrame();
						frameIncremented = true;
					}
					y += walkDistance;
				}
			}
		}
	}

	vector<DrawingInfo> drawingInfos;
	drawingInfos.push_back(createDrawingInfo(animationDirection, animation));

	if (attacking)
	{
		drawingInfos.push_back(createDrawingInfo(swordAnimation.directions[getDirectionIndex()], &swordAnimation));
	}

	return drawingInfos;
}