#pragma once

#include "BUILD_OPTIONS.h"
#include "Platform.h"

#if VK_USE_PLATFORM_WIN32_KHR

enum MouseButton
{
	LEFT = GLFW_MOUSE_BUTTON_LEFT,
	RIGHT = GLFW_MOUSE_BUTTON_RIGHT
};

#elif USE_FRAMEWORK_GLFW

enum MouseButton
{
	LEFT = GLFW_MOUSE_BUTTON_LEFT,
	RIGHT = GLFW_MOUSE_BUTTON_RIGHT
};

#endif