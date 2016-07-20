#include "VulkanPresentationSurface.h"

#if USE_FRAMEWORK_GLFW

VulkanPresentationSurface::VulkanPresentationSurface(VkInstance instance, GLFWwindow& window)
{
	if (glfwCreateWindowSurface(instance, &window, nullptr, &m_surface) != VK_SUCCESS)
	{
		glfwTerminate();
	}
}

VulkanPresentationSurface::~VulkanPresentationSurface()
{

}

#endif