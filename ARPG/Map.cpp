#include "json.hpp"
#include "Map.h"
#include <fstream>
#include <string>

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

json findSprite(json j, std::string id)
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

void Map::init(std::string surfaceName, std::string fileName, SurfaceLoader* surfaceLoader, SDL_Surface* windowSurface, int screenWidth, int screenHeight)
{
	surface = surfaceLoader->loadSurface(surfaceName, windowSurface);

	tiles.clear();

	std::ifstream i(fileName);
	json j;
	i >> j;

	auto& layer1 = j["layer1"];
	json layer1Sprite = findSprite(j, layer1["id"]);

	int width;
	int height;
	width = height = layer1Sprite["size"];

	for (int i = 0; i < screenWidth / width; ++i)
	{
		for (int j = 0; j < screenHeight / height; ++j)
		{
			DrawingInfo drawingInfo = createDrawingInfo(layer1Sprite, surface, i * width, j * height);
			Tile tile;
			tile.drawingInfo = drawingInfo;
			tile.collision = layer1Sprite["collision"];
			tiles.push_back(tile);
		}
	}

	for (auto& layer2 : j["layer2"])
	{
		json layer2Sprite = findSprite(j, layer2["id"]);
		DrawingInfo drawingInfo = createDrawingInfo(layer2Sprite, surface, layer2["x"], layer2["y"]);
		Tile tile;
		tile.drawingInfo = drawingInfo;
		tile.collision = layer2Sprite["collision"];
		tiles.push_back(tile);
	}
}