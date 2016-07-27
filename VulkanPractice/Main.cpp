
#include "Application.h"
#include "Game.h"
#include "Screen.h"
#include "TestScreen.h"

int main()
{
	Screen *screen = new TestScreen();
	Game *game = new Game(*screen);
	Application *application = new Application(*game, "Test Screen", 640, 480);

	application->start();

	delete screen;
	delete game;
	delete application;

	return 0;
}