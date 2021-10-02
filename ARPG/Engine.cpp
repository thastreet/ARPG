#include "Engine.h"
#include "Drawing.h"
#include "Link.h"
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <stdio.h>
#include <vector>

const int FPS = 60;
const int frameDelay = 1000 / FPS;
Uint32 frameStart;
int frameTime;
int totalFrame;
Link link = Link();
SurfaceLoader surfaceLoader = SurfaceLoader();
AnimationLoader animationLoader = AnimationLoader();

void mainLoop(SDL_Window* window, SDL_Surface* windowSurface, SDL_Surface* lightWorld) {
	bool quit = false;

	SDL_Event e;

	while (!quit)
	{
		frameStart = SDL_GetTicks();

		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			default:
				break;
			}
		}

		const Uint8* keyState = SDL_GetKeyboardState(NULL);

		SDL_Rect src = SDL_Rect();
		src.x = 0;
		src.y = 0;
		src.w = 8;
		src.h = 8;

		for (int i = 0; i < 100; ++i) {
			for (int j = 0; j < 100; ++j) {
				SDL_Rect dst = SDL_Rect();
				dst.x = i * 8;
				dst.y = j * 8;
				dst.w = 8;
				dst.h = 8;

				SDL_BlitSurface(lightWorld, &src, windowSurface, &dst);
			}
		}

		vector<DrawingInfo> linkDrawingInfos = link.handleKeyState(keyState, totalFrame);
		for (DrawingInfo drawingInfo : linkDrawingInfos)
		{
			SDL_BlitSurface(drawingInfo.surface, &drawingInfo.srcRect, windowSurface, &drawingInfo.dstRect);
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
	if (windowSurface == NULL)
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		std::vector<Drawable*> drawables;
		drawables.push_back(&link);

		SDL_Surface* lightWorld = surfaceLoader.loadSurface("light_world.png", windowSurface);

		for (Drawable* drawable : drawables)
		{
			drawable->init(windowSurface, &surfaceLoader, &animationLoader);
		}

		mainLoop(window, windowSurface, lightWorld);

		for (Drawable* drawable : drawables)
		{
			for (SDL_Surface* surface : drawable->getSurfaces())
			{
				SDL_FreeSurface(surface);
				surface = NULL;
			}
		}

		SDL_FreeSurface(lightWorld);
		lightWorld = NULL;
	}

	SDL_DestroyWindow(window);
	window = NULL;

	IMG_Quit();
	SDL_Quit();
}