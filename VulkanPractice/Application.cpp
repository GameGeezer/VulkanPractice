#include "Application.h"

#include "Game.h"

/*
KeyboardCallback* Application::KEYBOARD = new KeyboardCallback();

MouseClickCallback* Application::MOUSE_CLICK = new MouseClickCallback();

MouseMoveCallback* Application::MOUSE_MOVE = new MouseMoveCallback();

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	Application::KEYBOARD->invoke(key, scancode, action, mods);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	//if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	//popup_menu();
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	Application::MOUSE_CLICK->invoke(button, action, mods, xpos, ypos);
}

void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Application::MOUSE_MOVE->invoke(xpos, ypos);
}
*/

Application::Application(Game& game, string windowTitle, int windowWidth, int windowHeight) : m_game(&game), m_windowTitle(windowTitle), m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{

}

Application::~Application()
{

}

void
Application::start()
{
	this->init();

	this->m_game->onCreate();

	this->loop();
}

void
Application::init()
{
	
}

void
Application::loop()
{
	
	
}