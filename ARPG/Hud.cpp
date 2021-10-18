#include "Hud.h"

SDL_Surface* heartsSurface;

void Hud::init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader)
{
	heartsSurface = surfaceLoader->loadSurface("hearts.png", windowSurface);
}

std::vector<SDL_Surface*> Hud::getSurfaces()
{
	std::vector<SDL_Surface*> surfaces;
	surfaces.push_back(heartsSurface);
	return surfaces;
}

std::vector<DrawingInfo> Hud::tick(const Uint8* keyState, int totalFrame, std::vector<SDL_Rect> collisions, int screenWidth, int screenHeight)
{
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = 8;
	src.h = 8;

	std::vector<DrawingInfo> drawingInfos;

	for (int i = 0; i < 3; ++i)
	{
		SDL_Rect dst;
		dst.x = screenWidth - 16 - 3 * 8 + i * 8;
		dst.y = 8;
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