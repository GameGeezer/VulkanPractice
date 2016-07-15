#include "BUILD_OPTIONS.h"
#include "Platform.h"

#include "Window.h"
#include "Renderer.h"

#include <assert.h>
#include <iostream>

#if USE_FRAMEWORK_GLFW

void Window::initOSWindow()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	m_glfwWindow = glfwCreateWindow(m_width, m_height, m_name.c_str(), nullptr, nullptr);

	glfwGetFramebufferSize(m_glfwWindow, (int*)&m_width, (int*)&m_height);
}

void Window::deInitOSWindow()
{
	glfwDestroyWindow(m_glfwWindow);
}

void Window::updateOSWindow()
{
	glfwPollEvents();

	if (glfwWindowShouldClose(m_glfwWindow))
	{
		requestClose();
	}
}

void Window::initOSSurface()
{
	if (VK_SUCCESS != glfwCreateWindowSurface(m_renderer->GetVulkanInstance(), m_glfwWindow, nullptr, &m_surface))
	{
		glfwTerminate();

		assert(0 && "GLFW could not create the window surface.");

		return;
	}
}

#endif // USE_FRAMEWORK_GLFW