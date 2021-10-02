#include "Enemy.h"

using namespace std;

Animation mainAnimation = Animation();

void Enemy::init(SDL_Surface* windowSurface, SurfaceLoader* surfaceLoader, AnimationLoader* animationLoader)
{
	animationLoader->loadAnimation(&mainAnimation, "tentacle.json", "walking", surfaceLoader->loadSurface("tentacle.png", windowSurface));
}

vector<SDL_Surface*> Enemy::getSurfaces()
{
	vector<SDL_Surface*> surfaces;
	surfaces.push_back(mainAnimation.surface);
	return surfaces;
}

vector<DrawingInfo> Enemy::tick(const Uint8* keyState, int totalFrame)
{
	vector<DrawingInfo> drawingInfos;

	return drawingInfos;
}