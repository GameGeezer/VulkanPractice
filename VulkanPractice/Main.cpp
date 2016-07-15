#include "Renderer.h"

#include "Application.h"
#include "Game.h"
#include "Screen.h"
#include "TestScreen.h"

int main()
{
	Screen *screen = new TestScreen();
	Game *game = new Game(*screen);
	Application *application = new Application(*game, "Test Screen", 512, 512);

	application->start();

	return 0;
}