#include "Engine.h"
#include "Enemy.h"
#include "Hud.h"
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
const int linkDamageThreshold = 60;

Uint32 frameStart;
int frameTime;
int totalFrame;
int linkLastDamageFrame;

SurfaceLoader surfaceLoader = SurfaceLoader();
AnimationLoader animationLoader = AnimationLoader();

std::vector<Character*> characters;
std::vector<Enemy*> enemies;

void draw(DrawingInfo* drawingInfo, SDL_Surface* windowSurface)
{
	SDL_Rect* src = &drawingInfo->srcRect;
	SDL_Rect* dst = &drawingInfo->dstRect;
	SDL_BlitSurface(drawingInfo->surface, src, windowSurface, dst);
}

void draw(std::vector<DrawingInfo> drawingInfos, SDL_Surface* windowSurface)
{
	for (auto drawingInfo = drawingInfos.begin(); drawingInfo != drawingInfos.end(); ++drawingInfo)
	{
		draw(&(*drawingInfo), windowSurface);
	}
}

void drawCharacters(SDL_Surface* windowSurface, const Uint8* keyState, std::vector<SDL_Rect>* collisions, std::vector<Character*>* characters)
{
	for (Character* character : *characters)
	{
		std::vector<DrawingInfo> drawingInfos = character->tick(keyState, totalFrame, *collisions);
		for (auto drawingInfo = drawingInfos.begin(); drawingInfo != drawingInfos.end(); ++drawingInfo)
		{
			draw(&(*drawingInfo), windowSurface);

			if (debug)
			{
				SDL_Rect position = SDL_Rect();
				position.x = drawingInfo->dstRect.x;
				position.y = drawingInfo->dstRect.y;
				position.w = 2;
				position.h = 2;
				SDL_FillRect(windowSurface, &position, SDL_MapRGB(windowSurface->format, 255, 0, 0));

				SDL_Rect hitRect = character->getHitRect();
				SDL_FillRect(windowSurface, &hitRect, SDL_MapRGB(windowSurface->format, 0, 0, 255));
			}
		}
	}
}

void checkForLinkDamage(Link* link, Hud* hud)
{
	std::vector<SDL_Rect> enemiesCollisions;
	for (auto enemy = enemies.begin(); enemy != enemies.end(); ++enemy)
	{
		SDL_Rect hitRect = (*enemy)->getHitRect();
		enemiesCollisions.push_back(hitRect);
	}

	if (link->intersectsAnyCollision(enemiesCollisions))
	{
		if (linkLastDamageFrame % linkDamageThreshold == 0 && link->life >= 0)
		{
			--link->life;
			hud->update(link->life);
		}

		++linkLastDamageFrame;
	}
	else
	{
		linkLastDamageFrame = 0;
	}
}

void mainLoop(int screenWidth, int screenHeight, SDL_Window* window, SDL_Surface* windowSurface, std::vector<Character*>* characters, Map* map, Hud* hud, Link* link) {
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
			DrawingInfo drawingInfo = tile->drawingInfo;
			draw(&drawingInfo, windowSurface);
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

		drawCharacters(windowSurface, keyState, &collisions, characters);

		draw(hud->tick(keyState, totalFrame, collisions), windowSurface);

		SDL_UpdateWindowSurface(window);

		frameTime = SDL_GetTicks() - frameStart;
		if (frameDelay > frameTime)
		{
			SDL_Delay(frameDelay - frameTime);
		}
		++totalFrame;

		checkForLinkDamage(link, hud);
	}
}

void freeSurfaces(std::vector<Character*>* characters, Map* map)
{
	for (Character* character : *characters)
	{
		for (SDL_Surface* surface : character->getSurfaces())
		{
			SDL_FreeSurface(surface);
			surface = nullptr;
		}
	}


	SDL_FreeSurface(map->surface);
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
			character->init(windowSurface, &surfaceLoader, &animationLoader, screenWidth, screenHeight);
		}

		Map map;
		map.init("light_world.tiles.png", "map.json", &surfaceLoader, windowSurface, screenWidth, screenHeight);

		Hud hud;
		hud.init(windowSurface, &surfaceLoader, &animationLoader, screenWidth, screenHeight);
		hud.update(link.life);

		mainLoop(screenWidth, screenHeight, window, windowSurface, &characters, &map, &hud, &link);

		freeSurfaces(&characters, &map);
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