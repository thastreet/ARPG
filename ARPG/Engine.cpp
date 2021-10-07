#include "Engine.h"
#include "Drawing.h"
#include "Link.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>
#include <vector>
#include "Enemy.h"
#include "Map.h"

using namespace std;

const int FPS = 60;
const int frameDelay = 1000 / FPS;
const bool debug = false;

Uint32 frameStart;
int frameTime;
int totalFrame;

SurfaceLoader surfaceLoader = SurfaceLoader();
AnimationLoader animationLoader = AnimationLoader();

vector<Drawable*> drawables;
vector<Enemy*> enemies;

void mainLoop(int screenWidth, int screenHeight, SDL_Window* window, SDL_Surface* windowSurface, vector<Drawable*>* drawables, Map* map) {
	bool quit = false;

	SDL_Event event;

	while (!quit)
	{
		frameStart = SDL_GetTicks();

		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			default:
				break;
			}
		}

		Uint32 skyblue = SDL_MapRGB(windowSurface->format, 65, 193, 193);
		SDL_FillRect(windowSurface, nullptr, skyblue);

		vector<DrawingInfo> drawingInfos = map->drawingInfos;
		for (DrawingInfo drawingInfo : drawingInfos)
		{
			SDL_BlitSurface(drawingInfo.surface, &drawingInfo.srcRect, windowSurface, &drawingInfo.dstRect);
		}

		const Uint8* keyState = SDL_GetKeyboardState(nullptr);

		for (Drawable* drawable : *drawables)
		{
			vector<DrawingInfo> drawingInfos = drawable->tick(keyState, totalFrame);
			for (DrawingInfo drawingInfo : drawingInfos)
			{
				SDL_BlitSurface(drawingInfo.surface, &drawingInfo.srcRect, windowSurface, &drawingInfo.dstRect);

				if (debug)
				{
					SDL_Rect position = SDL_Rect();
					position.x = drawingInfo.dstRect.x;
					position.y = drawingInfo.dstRect.y;
					position.w = 2;
					position.h = 2;
					SDL_FillRect(windowSurface, &position, SDL_MapRGB(windowSurface->format, 255, 0, 0));

					SDL_Rect hitRect = drawable->getHitRect();
					SDL_FillRect(windowSurface, &hitRect, SDL_MapRGB(windowSurface->format, 0, 0, 255));
				}
			}
		}

		SDL_UpdateWindowSurface(window);

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
		++totalFrame;
	}
}

void Engine::start(int screenWidth, int screenHeight)
{
	SDL_Window* window = SDL_CreateWindow("ARPG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
	SDL_Surface* windowSurface = surfaceLoader.getWindowSurface(window);
	if (windowSurface == nullptr)
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		Enemy enemy = Enemy();
		enemies.push_back(&enemy);

		Link link = Link();
		link.attackAware = this;

		drawables.clear();
		drawables.push_back(&enemy);
		drawables.push_back(&link);

		for (Drawable* drawable : drawables)
		{
			drawable->init(windowSurface, &surfaceLoader, &animationLoader);
		}

		Map map;
		map.init("light_world.tiles.png", "map.json", &surfaceLoader, windowSurface);

		mainLoop(screenWidth, screenHeight, window, windowSurface, &drawables, &map);

		for (Drawable* drawable : drawables)
		{
			for (SDL_Surface* surface : drawable->getSurfaces())
			{
				SDL_FreeSurface(surface);
				surface = nullptr;
			}
		}


		SDL_FreeSurface(map.surface);
	}

	SDL_DestroyWindow(window);
	window = nullptr;

	IMG_Quit();
	SDL_Quit();
}

vector<Enemy*>::iterator removeEnemy(vector<Enemy*>::iterator enemy)
{
	for (auto drawable = drawables.begin(); drawable != drawables.end();)
	{
		if (*drawable == *enemy)
		{
			drawable = drawables.erase(drawable);
		}
		else
		{
			++drawable;
		}
	}

	return enemies.erase(enemy);
}

void Engine::attack(int x, int y)
{
	SDL_Point point = SDL_Point();
	point.x = x;
	point.y = y;

	for (vector<Enemy*>::iterator enemy = enemies.begin(); enemy != enemies.end();)
	{
		SDL_Rect hitRect = (*enemy)->getHitRect();

		if (SDL_PointInRect(&point, &hitRect))
		{
			(*enemy)->attack(1);
			if ((*enemy)->life <= 0)
			{
				enemy = removeEnemy(enemy);
			}
			else
			{
				++enemy;
			}
		}
		else
		{
			++enemy;
		}
	}
}