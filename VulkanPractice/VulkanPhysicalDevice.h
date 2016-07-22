#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanPhysicalDevice
{
public:
	VulkanPhysicalDevice(VkPhysicalDevice physicalDevice);

	~VulkanPhysicalDevice();

	bool
	hasQueueFamilyWithFlag(VkQueueFlagBits flagBit, uint32_t &out_queueIndex);

	VkPhysicalDevice
	getHandle();

private:
	VkPhysicalDevice						m_physicalDevice;
	uint32_t								m_queueFamilyPropertyCount = 0;
	std::vector<VkQueueFamilyProperties>	m_queueFamilyProperties;
};