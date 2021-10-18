#include "Engine.h"

const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;


int main(int argc, char* args[])
{
	Engine engine = Engine();

	engine.start(SCREEN_WIDTH, SCREEN_HEIGHT);

	return 0;
}