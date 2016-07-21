#ifndef VALIS_APPLICATION_H
#define VALIS_APPLICATION_H

#include <string>
#include <chrono>
#include <thread>
#include <iostream>

//#include "KeyboardCallback.cuh"
//#include "MouseClickCallback.cuh"
//#include "MouseMoveCallback.cuh"

class Game;

using namespace std;
using namespace std::chrono;

class Application
{
public:
	//static KeyboardCallback* KEYBOARD;
	//static MouseClickCallback* MOUSE_CLICK;
	//static MouseMoveCallback* MOUSE_MOVE;

	Application(Game& game, string windowTitle, int windowWidth, int windowHeight);
	~Application();

	void
		start();

private:

	int				m_windowWidth, m_windowHeight;
	Game*			m_game;
	string			m_windowTitle;
	
	milliseconds	lastFrameTime;

	void
		init();

	void
		loop();
};



#endif