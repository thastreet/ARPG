#include "Map.h"
#include "json.hpp"
#include <fstream>

using namespace nlohmann;

void Map::init(string surfaceName, string fileName, SurfaceLoader* surfaceLoader, SDL_Surface* windowSurface)
{
	SDL_Surface* surface = surfaceLoader->loadSurface(surfaceName, windowSurface);

	drawingInfos.clear();

	ifstream i(fileName);
	json j;
	i >> j;

	for (auto& placement : j["map"]) {
		DrawingInfo drawingInfo;

		SDL_Rect src = SDL_Rect();
		for (auto& sprite : j["sprites"])
		{
			if (sprite["id"] == placement["id"])
			{
				src.x = sprite["originX"];
				src.y = sprite["originY"];
				src.w = 16;
				src.h = 16;
				break;
			}
		}

		SDL_Rect dst = SDL_Rect();
		dst.x = placement["x"];
		dst.y = placement["y"];
		dst.w = 16;
		dst.h = 16;

		drawingInfo.srcRect = src;
		drawingInfo.dstRect = dst;
		drawingInfo.surface = surface;
		drawingInfos.push_back(drawingInfo);
	}
}