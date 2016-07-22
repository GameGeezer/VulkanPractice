#pragma once

#include <vulkan\vulkan.h>

class VulkanCommandPool
{
public:
	VulkanCommandPool(VkDevice device, bool transient, bool individual, uint32_t queueFamilyIndex);

	~VulkanCommandPool();

	void
	reset(bool releaseResources);

	VkCommandPool
	getHandle();

private:
	VkDevice		m_device;
	VkCommandPool	m_commandPool;
};