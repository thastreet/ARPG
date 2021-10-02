#include "Engine.h"

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


int main(int argc, char* args[])
{
	Engine engine = Engine();

	engine.start(SCREEN_WIDTH, SCREEN_HEIGHT);

	return 0;
}