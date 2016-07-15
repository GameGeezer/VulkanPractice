#pragma once

#include <vulkan\vulkan.h>

class VulkanQueueFamily
{
public:
	VkQueueFamilyProperties		queueFamilyProperties;

	VulkanQueueFamily(VkPhysicalDevice physicalDevice);

	~VulkanQueueFamily();
};