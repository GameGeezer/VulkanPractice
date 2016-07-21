#pragma once

#include <vulkan\vulkan.h>

class VulkanDevice;
class VulkanPresentationSurface;

class VulkanSwapchain
{
public:
	VulkanSwapchain(VulkanDevice &device, VulkanPresentationSurface &surface, uint32_t imageCount);

	~VulkanSwapchain();

	VkSwapchainKHR
	getHandle();

	uint32_t
	getImageCount();

private:
	VulkanDevice				*	m_device;
	VulkanPresentationSurface	*	m_surface;
	VkSwapchainKHR					m_swapchain;
	uint32_t						m_imageCount;
};