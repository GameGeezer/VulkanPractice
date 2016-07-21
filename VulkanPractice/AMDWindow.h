#ifndef AMD_VULKAN_SAMPLE_WINDOW_H_
#define AMD_VULKAN_SAMPLE_WINDOW_H_

#include <string>
#include <Windows.h>
#include <memory>

#include "Platform.h"

class VulkanDevice;
class VulkanPresentationSurface;
class VulkanSwapchain;

namespace AMD
{

	struct IWindow
	{
	public:
		IWindow() = default;
		IWindow(const IWindow&) = delete;
		IWindow& operator=(const IWindow&) = delete;

		virtual ~IWindow();

		bool IsClosed() const;
		virtual void OnClose() = 0;

	private:
		virtual bool IsClosedImpl() const = 0;
	};

	class Window : public IWindow
	{
	public:
		Window(VkInstance instance, VulkanDevice& device, char *title, uint32_t width, uint32_t height, uint32_t swapchainImageCount);

		~Window();

		void OnClose() override
		{
			isClosed_ = true;
		}


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

		VkImage
		getSwapchainImage(uint32_t index);

		VkImageView
		getSwapchainImageView(uint32_t index);

	private:
		bool IsClosedImpl() const override
		{
			return isClosed_;
		}

		// OS independent methods

		void
		initSwapchainImages();

		void
		deInitSwapchainImages();

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

		uint32_t		m_width;
		uint32_t		m_height;
		char *			m_title;

		VkInstance		m_vulkanInstance;

		VulkanPresentationSurface	*	m_surface;

		VulkanDevice *	m_device;

		VulkanSwapchain			*	m_swapchain;
		std::vector<VkImage>		m_swapchainImages;
		std::vector<VkImageView>	m_swapchainImageViews;
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
		

	};

}   // namespace AMD

#endif