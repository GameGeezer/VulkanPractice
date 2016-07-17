#pragma once

#include <vulkan\vulkan.h>

#include "Platform.h"

class Surface
{
public:
	Surface();

	~Surface();

#ifdef USE_FRAMEWORK_GLFW
	void
	initialize();
#elif VK_USE_PLATFORM_WIN32_KHR
	void
	initialize(VkInstance instance, HWND hwnd);
#endif
};