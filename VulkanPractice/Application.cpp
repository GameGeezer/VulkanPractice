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
	m_renderer = new Renderer();

	//m_window = new Window(m_renderer, m_windowWidth, m_windowHeight, m_windowTitle);
	/*
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), NULL, NULL);

	if (!window)
	{
		glfwTerminate();

		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	glfwSetKeyCallback(window, key_callback);

	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetCursorPosCallback(window, mouse_position_callback);

	glewInit();
	*/
}

void
Application::loop()
{
	
	while (m_window->isAlive())
	{
		milliseconds currentFrame = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
		milliseconds delta = currentFrame - lastFrameTime;
		lastFrameTime = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

		m_game->update((int32_t) delta.count());

		m_window->update();

		Sleep(60);
	}
	
	
	/*
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		milliseconds currentFrame = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
		milliseconds delta = currentFrame - lastFrameTime;

		std::cout << delta.count() << endl;
		game->update(delta.count());

		glfwSwapBuffers(window);

		glfwPollEvents();

		lastFrameTime = currentFrame;
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	*/
}