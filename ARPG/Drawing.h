#pragma once
#include <SDL.h>
#include <vector>
#include <string>

using namespace std;

struct DrawingInfo
{
	SDL_Rect srcRect;
	SDL_Rect dstRect;
	SDL_Surface* surface;
};

class SurfaceLoader
{
public:
	SDL_Surface* getWindowSurface(SDL_Window* window);
	SDL_Surface* loadSurface(std::string path, SDL_Surface* windowSurface);
};

const int DIRECTIONS_SIZE = 4;

struct AnimationDirection {
	int x;
	int y;
	int width;
	int height;
	int offsetX;
	int offsetY;
	int size;
};

struct Animation {
	AnimationDirection directions[DIRECTIONS_SIZE];
	SDL_Surface* surface;
};

class AnimationLoader
{
public:
	void loadAnimation(Animation* animation, string name, string key, SDL_Surface* surface);
};

class Drawable
{
public:
	virtual void init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader) = 0;
	virtual vector<SDL_Surface*> getSurfaces() = 0;
};

enum class Direction
{
	UP,
	LEFT,
	RIGHT,
	DOWN
};