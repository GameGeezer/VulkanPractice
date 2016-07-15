#pragma once

// Contact me if you're interested about adding platform
// support in these tutorials, contact me on my youtube channel:
// https://www.youtube.com/user/Nigo40
// Biggest missing right now is support for Android.
// others like Xlib and so are also welcome additions.

#include "BUILD_OPTIONS.h"
#include <stdint.h>
#include <vector>

void InitPlatform();

void DeInitPlatform();

void AddRequiredPlatformInstanceExtensions(std::vector<const char *> *instance_extensions);

// GLFW
#if BUILD_USE_GLFW

// Define as a build option 
#define USE_FRAMEWORK_GLFW 1
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "PlatformHeaders_glfw.h"
// For Windows Message Box
#if defined( _WIN32 )
#undef APIENTRY
#include <windows.h>
#endif 

// WINDOWS
#elif defined( _WIN32 )
// this is always defined on windows platform

#define VK_USE_PLATFORM_WIN32_KHR 1
#include <windows.h>
#include "PlatformHeaders_win32.h"
#else
// platform not yet supported
#error Platform not yet supported
#endif

#include <vulkan/vulkan.h>