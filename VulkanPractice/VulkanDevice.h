#pragma once

#include <vulkan\vulkan.h>
#include <stdint.h>
#include <vector>

using namespace std;

class VulkanDevice
{
public:

	const VkDevice						device;
	const VkPhysicalDevice				physicalDevice;
	const VkPhysicalDeviceProperties	physicalDeviceProperties;

	const uint32_t						queueFamilyCount;
	const VkQueueFamilyProperties		queueFamilyProperties;

	VulkanDevice(VkDevice device, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties physicalDeviceProperties, uint32_t queueFamilyCount, VkQueueFamilyProperties queueFamilyProperties);

	~VulkanDevice();

	void getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* queue);
};