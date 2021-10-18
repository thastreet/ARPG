#pragma once
#include "DrawingInfo.h"
#include "SurfaceLoader.h"
#include <SDL.h>
#include <string>
#include <vector>

struct Tile
{
	DrawingInfo drawingInfo;
	bool collision;
};

class Map
{
public:
	void init(std::string surfaceName, std::string fileName, SurfaceLoader* surfaceLoader, SDL_Surface* windowSurface, int screenWidth, int screenHeight);

	std::vector<Tile> tiles;
	SDL_Surface* surface = nullptr;
};