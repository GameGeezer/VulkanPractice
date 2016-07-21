#include "AMDWindow.h"

#include "Shared.h"

#include "VulkanDevice.h"
#include "VulkanPresentationSurface.h"
#include "VulkanSwapChain.h"

namespace AMD
{
		///////////////////////////////////////////////////////////////////////////////
	IWindow::~IWindow()
	{
		
	}

	///////////////////////////////////////////////////////////////////////////////
	Window::Window(VkInstance instance, VulkanDevice& device, char *title, const uint32_t width, const uint32_t height, uint32_t swapchainImageCount): m_vulkanInstance(instance), m_device(&device), m_title(title), m_width(width), m_height(height), m_swapchainImageCount(swapchainImageCount)
	{
		initOsWindow();
		m_surface = new VulkanPresentationSurface(*m_device, m_vulkanInstance, initOSSurface());
		m_swapchain = new VulkanSwapchain(*m_device, *m_surface, m_swapchainImageCount);
	//	initSwapchainImages();
	}

	Window::~Window()
	{
		deInitSwapchainImages();
		delete m_swapchain;
		delete m_surface;
		deInitOSWindow();
	}

	///////////////////////////////////////////////////////////////////////////////
	bool IWindow::IsClosed() const
	{
		return IsClosedImpl();
	}

	void Window::initSwapchainImages()
	{
		m_swapchainImages.resize(m_swapchainImageCount);
		m_swapchainImageViews.resize(m_swapchainImageCount);

		ErrorCheck(vkGetSwapchainImagesKHR(m_device->getDevice(), m_swapchain->getHandle(), &m_swapchainImageCount, m_swapchainImages.data()));

		for (uint32_t i = 0; i < m_swapchainImageCount; ++i) {
			VkImageViewCreateInfo image_view_create_info{};
			image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			image_view_create_info.image = m_swapchainImages[i];
			image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
			image_view_create_info.format = m_surface->getFormat()->format;
			image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			image_view_create_info.subresourceRange.baseMipLevel = 0;
			image_view_create_info.subresourceRange.levelCount = 1;
			image_view_create_info.subresourceRange.baseArrayLayer = 0;
			image_view_create_info.subresourceRange.layerCount = 1;

			ErrorCheck(vkCreateImageView(m_device->getDevice(), &image_view_create_info, nullptr, &m_swapchainImageViews[i]));
		}
	}

	void Window::deInitSwapchainImages()
	{
		for (auto view : m_swapchainImageViews) {
			vkDestroyImageView(m_device->getDevice(), view, nullptr);
		}
	}


	VkSurfaceKHR
	Window::getSurface()
	{
		return m_surface->getHandle();
	}

	VkSwapchainKHR
	Window::getSwapchain()
	{
		return m_swapchain->getHandle();
	}


	///////////////////////////////////////////////////////////////////////////////
	uint32_t
	Window::getWidth()
	{
		return m_width;
	}

	///////////////////////////////////////////////////////////////////////////////
	uint32_t
	Window::getHeight()
	{
		return m_height;
	}

	VkFormat
	Window::getSurfaceFormat()
	{
		return m_surface->getFormat()->format;
	}


	VkImage
	Window::getSwapchainImage(uint32_t index)
	{
		return m_swapchainImages.at(index);
	}

	VkImageView
	Window::getSwapchainImageView(uint32_t index)
	{
		return m_swapchainImageViews.at(index);
	}

}   // namespace AMD