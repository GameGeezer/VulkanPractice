#include "Screen.h"

bool
Screen::isCreated()
{
	return m_isCreated;
}

void
Screen::initialize(Application &application, Game &game)
{
	m_application = &application;

	m_game = m_game;
}

Application*
Screen::getApplication()
{
	return m_application;
}