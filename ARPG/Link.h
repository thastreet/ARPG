#pragma once

#include "AnimationLoader.h"
#include "AttackAware.h"
#include "Character.h"
#include "DrawingInfo.h"
#include "SurfaceLoader.h"

class Link : public Character
{
public:
	void init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader);
	std::vector<SDL_Surface*> getSurfaces();
	std::vector<DrawingInfo> tick(const Uint8* keyState, int totalFrame, std::vector<SDL_Rect> collisions, int screenWidth, int screenHeight);
	SDL_Rect getHitRect();

	AttackAware* attackAware = nullptr;

private:
	void finishAttackingIfNecessary();
	void continueAttacking();
	void handleKeyState(const Uint8* keyState, bool shouldAnimateWalking, std::vector<SDL_Rect> collisions);
	bool onLeftPressed(const Uint8* keyState, std::vector<SDL_Rect> collisions, bool frameAlreadyIncremented, bool shouldAnimateWalking);
	bool onRightPressed(const Uint8* keyState, std::vector<SDL_Rect> collisions, bool frameAlreadyIncremented, bool shouldAnimateWalking);
	bool onUpPressed(const Uint8* keyState, std::vector<SDL_Rect> collisions, bool frameAlreadyIncremented, bool shouldAnimateWalking);
	bool onDownPressed(const Uint8* keyState, std::vector<SDL_Rect> collisions, bool frameAlreadyIncremented, bool shouldAnimateWalking);
};

