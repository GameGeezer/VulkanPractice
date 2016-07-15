#pragma once

#include <vulkan\vulkan.h>
#include <stdint.h>
#include <vector>

using namespace std;

class VulkanDevice
{
public:

	VkDevice					device;
	VkPhysicalDevice			physicalDevice;
	VkPhysicalDeviceProperties	physicalDeviceProperties;

	uint32_t					queueFamilyCount;
	VkQueueFamilyProperties		queueFamilyProperties;

	VulkanDevice(VkPhysicalDevice physicalDevice, VkQueueFamilyProperties queueFamilyProperties);

	~VulkanDevice();

	void getGraphicsFamily();

private:
	
	vector<uint32_t> graphicsFamilyQueues;
};