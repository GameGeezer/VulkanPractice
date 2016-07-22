#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanPhysicalDevice;

class VulkanEnumeratedPhysicalDevices
{
public:
	VulkanEnumeratedPhysicalDevices(VkInstance instance);

	~VulkanEnumeratedPhysicalDevices();

	void
	findDevicesWithQueueFlag(VkQueueFlagBits flagBit, std::vector<VulkanPhysicalDevice*> &out_devices, std::vector<uint32_t> &out_indexes);

private:
	uint32_t m_physicalDeviceCount = -1;
	std::vector<VulkanPhysicalDevice*> m_physicalDevices;
};