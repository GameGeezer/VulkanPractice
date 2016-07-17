#include "Surface.h"

#if VK_USE_PLATFORM_WIN32_KHR

#include "BUILD_OPTIONS.h"
//#include "Window.h"
//#include "Renderer.h"

Surface::Surface()
{
	/*
	VkWin32SurfaceCreateInfoKHR win32surfaceCreateInfo = {};
	win32surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	win32surfaceCreateInfo.hwnd = hwnd;
	win32surfaceCreateInfo.hinstance = ::GetModuleHandle(nullptr);

	VkSurfaceKHR surface = nullptr;
	vkCreateWin32SurfaceKHR(instance, &win32surfaceCreateInfo, nullptr,
		&surface);
		*/
}

Surface::~Surface()
{

}

void
Surface::initialize(VkInstance instance, HWND hwnd)
{

}

#endif