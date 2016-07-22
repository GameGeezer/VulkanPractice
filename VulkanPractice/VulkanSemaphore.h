#pragma once

#include <vulkan\vulkan.h>

class VulkanSemaphore
{
public:
	VulkanSemaphore(VkDevice device);

	~VulkanSemaphore();

	VkSemaphore
	getHandle();

private:
	VkDevice	m_device;
	VkSemaphore m_semaphore;
};
