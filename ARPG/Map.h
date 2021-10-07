#pragma once
#include <vector>
#include "Drawing.h"

class Map
{
public:
	void init(string surfaceName, string fileName, SurfaceLoader* surfaceLoader, SDL_Surface* windowSurface);

	vector<DrawingInfo> drawingInfos;
	SDL_Surface* surface = nullptr;
};

