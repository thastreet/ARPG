#include "Engine.h"
#include "Enemy.h"
#include "Link.h"
#include "Map.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>
#include <vector>

const int FPS = 60;
const int frameDelay = 1000 / FPS;
const bool debug = false;

Uint32 frameStart;
int frameTime;
int totalFrame;

SurfaceLoader surfaceLoader = SurfaceLoader();
AnimationLoader animationLoader = AnimationLoader();

std::vector<Character*> characters;
std::vector<Enemy*> enemies;

void mainLoop(int screenWidth, int screenHeight, SDL_Window* window, SDL_Surface* windowSurface, std::vector<Character*>* characters, Map* map) {
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

		const std::vector<Tile> tiles = map->tiles;

		for (auto tile = tiles.begin(); tile != tiles.end(); ++tile)
		{
			SDL_Rect dstRect = tile->drawingInfo.dstRect;
			SDL_BlitSurface(tile->drawingInfo.surface, &tile->drawingInfo.srcRect, windowSurface, &dstRect);
		}

		const Uint8* keyState = SDL_GetKeyboardState(nullptr);

		std::vector<SDL_Rect> collisions;
		for (auto tile = tiles.begin(); tile != tiles.end(); ++tile)
		{
			if (tile->collision)
			{
				SDL_Rect rect = tile->drawingInfo.dstRect;
				collisions.push_back(rect);
			}
		}

		for (Character* character : *characters)
		{
			std::vector<DrawingInfo> drawingInfos = character->tick(keyState, totalFrame, collisions);
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

					SDL_Rect hitRect = character->getHitRect();
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

		characters.clear();
		characters.push_back(&enemy);
		characters.push_back(&link);

		for (Character* character : characters)
		{
			character->init(windowSurface, &surfaceLoader, &animationLoader);
		}

		Map map;
		map.init("light_world.tiles.png", "map.json", &surfaceLoader, windowSurface);

		mainLoop(screenWidth, screenHeight, window, windowSurface, &characters, &map);

		for (Character* character : characters)
		{
			for (SDL_Surface* surface : character->getSurfaces())
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

std::vector<Enemy*>::iterator removeEnemy(std::vector<Enemy*>::iterator enemy)
{
	for (auto character = characters.begin(); character != characters.end();)
	{
		if (*character == *enemy)
		{
			character = characters.erase(character);
		}
		else
		{
			++character;
		}
	}

	return enemies.erase(enemy);
}

void Engine::attack(int x, int y)
{
	SDL_Point point = SDL_Point();
	point.x = x;
	point.y = y;

	for (std::vector<Enemy*>::iterator enemy = enemies.begin(); enemy != enemies.end();)
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