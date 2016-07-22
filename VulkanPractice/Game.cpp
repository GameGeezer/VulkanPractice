#include "Game.h"

#include "Application.h"
#include "Screen.h"

Game::Game(Screen& initialScreen)
{
	m_currentScreen = &initialScreen;
}

void
Game::onCreate(Application &application)
{
	m_application = &application;

	m_currentScreen->initialize(application, *this);

	m_currentScreen->m_isCreated = true;

	m_currentScreen->onCreate();

	m_currentScreen->onResume();

	m_isCreated = true;
}

void
Game::update(uint32_t delta)
{
	m_currentScreen->onUpdate(delta);
}


void
Game::render(VulkanCommandBuffer *commandBuffer)
{
	m_currentScreen->onRender(commandBuffer);
}

void
Game::setScreen(Screen& screen)
{
	// Create the new screen if it hasn't been
	if (!screen.isCreated() && isCreated())
	{
		screen.onCreate();

		m_currentScreen->initialize(*m_application, *this);

		screen.m_isCreated = true;
	}

	// Resume the new screen
	if (isCreated())
	{
		m_currentScreen->onLeave();

		screen.onResume();
	}

	m_currentScreen = &screen;
}

bool
Game::isCreated()
{
	return m_isCreated;
}