#include "Hud.h"

SDL_Surface* heartsSurface;
int mScreenWidth;
int mLife;
const int maxUnlockedLife = 20;
const int maxLife = 20;
const int heartSize = 8;
const int heartsTopMargin = 8;
const int heartsRightMargin = 16;

SDL_Rect fullHeartRect;
SDL_Rect emptyHeartRect;

void Hud::init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader, int screenWidth, int screenHeight)
{
	heartsSurface = surfaceLoader->loadSurface("hearts.png", windowSurface);
	mScreenWidth = screenWidth;

	fullHeartRect.x = 0;
	fullHeartRect.y = 0;
	fullHeartRect.w = heartSize;
	fullHeartRect.h = heartSize;

	emptyHeartRect.x = 16;
	emptyHeartRect.y = 0;
	emptyHeartRect.w = heartSize;
	emptyHeartRect.h = heartSize;
}

std::vector<SDL_Surface*> Hud::getSurfaces()
{
	std::vector<SDL_Surface*> surfaces;
	surfaces.push_back(heartsSurface);
	return surfaces;
}

std::vector<DrawingInfo> Hud::tick(const Uint8* keyState, int totalFrame, std::vector<SDL_Rect> collisions)
{
	std::vector<DrawingInfo> drawingInfos;

	for (int i = 0; i < maxUnlockedLife; ++i)
	{
		int rowIndex = i / (maxLife / 2);
		int xOffset = rowIndex * heartSize * (maxLife / 2);
		int reversedColumnIndex = maxLife / 2 - 1 - i;

		int x = mScreenWidth - heartsRightMargin - reversedColumnIndex * heartSize - xOffset;
		int y = heartsTopMargin + rowIndex * heartSize;

		SDL_Rect dst;
		dst.x = x;
		dst.y = y;
		dst.w = heartSize;
		dst.h = heartSize;

		DrawingInfo drawingInfo;

		if (i < mLife)
		{
			drawingInfo.srcRect = fullHeartRect;
		}
		else
		{
			drawingInfo.srcRect = emptyHeartRect;
		}
		
		drawingInfo.dstRect = dst;
		drawingInfo.surface = heartsSurface;

		drawingInfos.push_back(drawingInfo);
	}

	return drawingInfos;
}

void Hud::update(int life)
{
	mLife = life;
}