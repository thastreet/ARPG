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

vector<SDL_Surface*> Link::getSurfaces()
{
	vector<SDL_Surface*> surfaces;
	surfaces.push_back(walkingAnimation.surface);
	surfaces.push_back(stoppedAnimation.surface);
	surfaces.push_back(attackingAnimation.surface);
	surfaces.push_back(swordAnimation.surface);
	return surfaces;
}

vector<DrawingInfo> Link::tick(const Uint8 * keyState, int totalFrame)
{
	const bool shouldAnimateWalking = totalFrame % walkingAnimationThreshold == 0;

	if (attacking && frame == animationDirection.size - 1)
	{
		attacking = false;
		frame = 0;
	}

	if (attacking)
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