#pragma once

#include <vulkan\vulkan.h>

#include "BUILD_OPTIONS.h"
#include "Platform.h"

class VulkanPresentationSurface
{
public:

#if USE_FRAMEWORK_GLFW
	VulkanPresentationSurface(VkInstance instance, GLFWwindow& window);
#elif VK_USE_PLATFORM_WIN32_KHR
	VulkanPresentationSurface();
#endif

	~VulkanPresentationSurface();

private:
	VkSurfaceKHR m_surface;
};