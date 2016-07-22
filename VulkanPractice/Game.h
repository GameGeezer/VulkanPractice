#ifndef VALIS_GAME_H
#define VALIS_GAME_H

#include <stdint.h>

class Screen;

class Game
{
	friend class Application;

public:

	Game(Screen& initialScreen);

	void
		setScreen(Screen& screen);

protected:

	void
		onCreate();

	void
		update(uint32_t delta);

private:

	bool isCreated;

	Screen* currentScreen;
};

#endif