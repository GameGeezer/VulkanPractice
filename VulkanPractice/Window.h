#pragma once

#include "Platform.h"

#include <vector>
#include <string>

#include <vulkan\vulkan.h>

class Renderer;

class Window
{
public:
	Window(Renderer * renderer, uint32_t size_x, uint32_t size_y, std::string name);
	~Window();

	void	requestClose();
	void	update();
	bool	isAlive();

private:
	void	initOSWindow();
	void	deInitOSWindow();
	void	updateOSWindow();
	void	initOSSurface();

	void	initSurface();
	void	deInitSurface();

	void	initSwapchain();
	void	deInitSwapchain();

	void	initSwapchainImages();
	void	deInitSwapchainImages();

//	void	keyDownEvent(KeyboardKeycode code);

	Renderer						*	m_renderer	= nullptr;

	uint32_t							m_width		= 512;
	uint32_t							m_height	= 512;
	std::string							m_name;

	VkSurfaceKHR						m_surface	= VK_NULL_HANDLE;
	VkSwapchainKHR						m_swapchain = VK_NULL_HANDLE;

	uint32_t							m_swapchainImageCount = 2;
	std::vector<VkImage>				m_swapchainImages;
	std::vector<VkImageView>			m_swapchainImageViews;

	VkSurfaceFormatKHR					_surface_format = {};
	VkSurfaceCapabilitiesKHR			_surface_capabilities = {};

	bool								m_isAlive = true;

#if USE_FRAMEWORK_GLFW
	GLFWwindow						*	m_glfwWindow = nullptr;

#elif VK_USE_PLATFORM_WIN32_KHR
	HINSTANCE							m_win32Instance = NULL;
	HWND								m_win32Window = NULL;
	std::string							m_win32ClassName;
	static uint64_t						m_win32ClassIdCounter;
#endif
};