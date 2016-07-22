#include "Platform.h"

#if USE_FRAMEWORK_GLFW

#include "Window.h"

//#include "VulkanPresentationSurface.h"

void
Window::initOsWindow()
{
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	m_glfwWindow = glfwCreateWindow(m_width, m_height, m_title, nullptr, nullptr);

	glfwGetFramebufferSize(m_glfwWindow, (int*)&m_width, (int*)&m_height);
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