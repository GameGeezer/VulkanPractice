#pragma once

#include <vulkan\vulkan.h>
#include <stdint.h>
#include <vector>

#include "VulkanDevice.h"

using namespace std;

class Renderer
{
public:

	Renderer();

	~Renderer();

	VulkanDevice* getDevice(int number);

private:

	void _initInstance();

	void _initDevices();

	void _createDevice(VkPhysicalDevice& physicalDevice, VkDevice& outDevice);

	void _createDeviceProperties(VkPhysicalDevice& device, VkPhysicalDeviceProperties& outProperties);

	void _dubugFamilyQueueCheckSupported(VkQueueFamilyProperties& familitProperties);

	
	VkInstance				instance;
	vector<VulkanDevice*>*	vulkanDevices;
	uint32_t				deviceCount = 0;
};
