#ifndef VALIS_GAME_H
#define VALIS_GAME_H

#include <stdint.h>

class Application;
class Screen;
class VulkanCommandBuffer;

class Game
{
	friend class Application;

public:

	Game(Screen& initialScreen);

	void
	setScreen(Screen& screen);

	bool
	isCreated();

protected:

	void
	onCreate(Application &application);

	void
	update(uint32_t delta);

	void
	render(VulkanCommandBuffer *commandBuffer);

private:

	bool			m_isCreated;

	Application	*	m_application;
	Screen		*	m_currentScreen;
};

#endif