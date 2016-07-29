#include "Platform.h"

#if USE_FRAMEWORK_GLFW

#include "Window.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, VK_TRUE);
	}

	switch (action)
	{
	case GLFW_REPEAT:

		break;
	case GLFW_PRESS:
		Window::KEYBOARD->onKeyPressed(key);
		break;
	case GLFW_RELEASE:
		Window::KEYBOARD->onKeyReleased(key);
		break;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	switch (action)
	{
	case GLFW_PRESS:
		Window::MOUSE_CLICK->onButtonPress(button, static_cast<float>(xpos), static_cast<float>(ypos));
		break;
	case GLFW_RELEASE:
		Window::MOUSE_CLICK->onButtonRelease(button, static_cast<float>(xpos), static_cast<float>(ypos));
		break;
	}
}

void mouse_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Window::MOUSE_MOVE->invoke(static_cast<float>(xpos), static_cast<float>(ypos));
}

void
Window::initOsWindow()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

	glfwGetFramebufferSize(m_glfwWindow, (int*)&m_width, (int*)&m_height);

	glfwSetKeyCallback(m_glfwWindow, key_callback);

	glfwSetMouseButtonCallback(m_glfwWindow, mouse_button_callback);

	glfwSetCursorPosCallback(m_glfwWindow, mouse_position_callback);
}

void
Window::deInitOSWindow()
{
	glfwDestroyWindow(m_glfwWindow);
}

void
Window::updateOSWindow()
{
	glfwPollEvents();

	if (glfwWindowShouldClose(m_glfwWindow))
	{
		requestClose();
	}

}

VkSurfaceKHR
Window::initOSSurface()
{
	VkSurfaceKHR surface;
	if (VK_SUCCESS != glfwCreateWindowSurface(m_vulkanInstance, m_glfwWindow, nullptr, &surface))
	{
		glfwTerminate();

		//	assert(0 && "GLFW could not create the window surface.");

		return VK_NULL_HANDLE;
	}

	return surface;
}
#endif