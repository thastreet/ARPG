#include "Map.h"
#include "json.hpp"
#include <fstream>

using namespace nlohmann;

DrawingInfo createDrawingInfo(json sprite, SDL_Surface* surface, int x, int y)
{
	DrawingInfo drawingInfo;
	SDL_Rect src = SDL_Rect();
	SDL_Rect dst = SDL_Rect();

	int width;
	int height;
	width = height = sprite["size"];

	src.x = sprite["originX"];
	src.y = sprite["originY"];
	src.w = width;
	src.h = height;

	dst.x = x;
	dst.y = y;
	dst.w = width;
	dst.h = height;

	drawingInfo.srcRect = src;
	drawingInfo.dstRect = dst;
	drawingInfo.surface = surface;

	return drawingInfo;
}

json findSprite(json j, string id)
{
	for (auto& sprite : j["sprites"])
	{
		if (sprite["id"] == id)
		{
			return sprite;
		}
	}

	throw;
}

void Map::init(string surfaceName, string fileName, SurfaceLoader* surfaceLoader, SDL_Surface* windowSurface)
{
	surface = surfaceLoader->loadSurface(surfaceName, windowSurface);

	drawingInfos.clear();

	ifstream i(fileName);
	json j;
	i >> j;

	auto& layer1 = j["layer1"];
	json layer1Sprite = findSprite(j, layer1["id"]);

	int width;
	int height;
	width = height = layer1Sprite["size"];

	for (int i = 0; i < 160 / width; ++i)
	{
		for (int j = 0; j < 160 / height; ++j)
		{
			DrawingInfo drawingInfo = createDrawingInfo(layer1Sprite, surface, i * width, j * height);
			drawingInfos.push_back(drawingInfo);
		}
	}

	for (auto& layer2 : j["layer2"])
	{
		json layer2Sprite = findSprite(j, layer2["id"]);
		DrawingInfo drawingInfo = createDrawingInfo(layer2Sprite, surface, layer2["x"], layer2["y"]);
		drawingInfos.push_back(drawingInfo);
	}
}