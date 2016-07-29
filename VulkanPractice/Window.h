#ifndef AMD_VULKAN_SAMPLE_WINDOW_H_
#define AMD_VULKAN_SAMPLE_WINDOW_H_

#include <string>
#include <Windows.h>
#include <memory>

#include "Platform.h"

#include "KeyboardCallback.h"
#include "MouseClickCallback.h"
#include "MouseMoveCallback.h"

class VulkanDevice;
class VulkanPresentationSurface;
class VulkanSwapchain;
class VulkanSwapchainImages;
class VulkanRenderPass;

class Window
{
public:
	static KeyboardCallback* KEYBOARD;
	static MouseClickCallback* MOUSE_CLICK;
	static MouseMoveCallback* MOUSE_MOVE;

	Window(VkInstance instance, VulkanDevice& device, char *title, uint32_t width, uint32_t height, uint32_t swapchainImageCount);

	~Window();

	uint32_t
	acquireNextImage(uint64_t timeout, VkSemaphore imageAcquiredSemaphore);

	void
	present();

	void
	update();

	void
	beginRenderPass(VkCommandBuffer commandBuffer, uint32_t index);

	void
	endRenderPass(VkCommandBuffer commandBuffer);

	VulkanRenderPass*
	getRenderPass();

	VkSurfaceKHR
	getSurface();

	VkSwapchainKHR
	getSwapchain();

	uint32_t
	getWidth();

	uint32_t
	getHeight();

	VkFormat
	getSurfaceFormat();

	bool
	isAlive();

	void
	requestClose();

private:

	// OS depentent methods

	void
	initOsWindow();

	void
	deInitOSWindow();

	void
	updateOSWindow();

	VkSurfaceKHR
	initOSSurface();

	// OS independent variables

	bool			m_isAlive = true;

	uint32_t		m_width;
	uint32_t		m_height;
	char *			m_title;

	VkInstance		m_vulkanInstance;

	VulkanPresentationSurface	*	m_surface;
	VulkanDevice				*	m_device;
	VulkanSwapchain				*	m_swapchain;
	VulkanSwapchainImages		*	m_swapchainImages;
	VulkanRenderPass					*	m_renderPass;

	uint32_t					m_swapchainImageCount;

	// Os dependent variables

#if USE_FRAMEWORK_GLFW
	GLFWwindow	*	m_glfwWindow = nullptr;

#elif VK_USE_PLATFORM_WIN32_KHR
	HINSTANCE		m_win32Instance = NULL;
	HWND			m_win32Window = NULL;
	std::string		m_win32ClassName;
	static uint64_t	m_win32ClassIdCounter;
#endif

	bool isClosed_ = false;


};  // namespace AMD

#endif