#pragma once
#include <vector>
#include "Drawing.h"

struct Tile
{
	DrawingInfo drawingInfo;
	bool collision;
};

class Map
{
public:
	void init(string surfaceName, string fileName, SurfaceLoader* surfaceLoader, SDL_Surface* windowSurface);

	vector<Tile> tiles;
	SDL_Surface* surface = nullptr;
};