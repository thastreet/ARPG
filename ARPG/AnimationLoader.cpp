#include "Animation.h"
#include "AnimationDirection.h"
#include "AnimationLoader.h"
#include "json.hpp"
#include <fstream>
#include <string>

using namespace nlohmann;

AnimationDirection createAnimationDirection(json direction)
{
	AnimationDirection animationDirection = AnimationDirection();
	animationDirection.x = direction["x"];
	animationDirection.y = direction["y"];
	animationDirection.width = direction["frame_width"];
	animationDirection.height = direction["frame_height"];
	animationDirection.offsetX = direction["origin_x"];
	animationDirection.offsetY = direction["origin_y"];
	if (direction.contains("num_frames")) {
		animationDirection.size = direction["num_frames"];
	}
	else {
		animationDirection.size = 1;
	}
	return animationDirection;
}

void AnimationLoader::loadAnimation(Animation* animation, std::string name, std::string key, SDL_Surface* surface)
{
	animation->surface = surface;

	std::ifstream i(name);
	json j;
	i >> j;

	for (auto& element : j) {
		if (element["name"] == key) {
			json directions = element["directions"];
			for (int i = 0; i < DIRECTIONS_SIZE; ++i) {
				animation->directions[i] = createAnimationDirection(directions[i]);
			}
			break;
		}
	}
}