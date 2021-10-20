#include "Hud.h"

SDL_Surface* heartsSurface;
int mScreenWidth;
int mLife;
const int heartSize = 8;
const int heartsTopMargin = 8;
const int heartsRightMargin = 16;

void Hud::init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader, int screenWidth, int screenHeight)
{
	heartsSurface = surfaceLoader->loadSurface("hearts.png", windowSurface);
	mScreenWidth = screenWidth;
}

std::vector<SDL_Surface*> Hud::getSurfaces()
{
	std::vector<SDL_Surface*> surfaces;
	surfaces.push_back(heartsSurface);
	return surfaces;
}

std::vector<DrawingInfo> Hud::tick(const Uint8* keyState, int totalFrame, std::vector<SDL_Rect> collisions)
{
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = 8;
	src.h = 8;

	std::vector<DrawingInfo> drawingInfos;

	for (int i = 0; i < mLife; ++i)
	{
		const int start = mScreenWidth - heartsRightMargin - mLife * heartSize;

		SDL_Rect dst;
		dst.x = start + (mLife - i) * heartSize;
		dst.y = heartsTopMargin;
		dst.w = 8;
		dst.h = 8;

		DrawingInfo drawingInfo;
		drawingInfo.srcRect = src;
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