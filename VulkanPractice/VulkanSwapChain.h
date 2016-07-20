#pragma once

#include <vulkan\vulkan.h>

class VulkanSwapchain
{
public:
	VulkanSwapchain();

	~VulkanSwapchain();

private:
	VkSwapchainKHR m_swapChain;
};