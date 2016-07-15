#include "VulkanDevice.h"

VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice, VkQueueFamilyProperties queueFamilyProperties)
{
	// Get the device properties
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);
	// Get the queue family count
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
	// Get the queue family properties
	std::vector<VkQueueFamilyProperties> familyProperties(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, familyProperties.data());
	// Most vendor implementations group all queues in to one family
	for (uint32_t i = 0; i < queueFamilyCount; ++i)
	{
		if (familyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			graphicsFamilyQueues.push_back(i);
		}
	}
}

VulkanDevice::~VulkanDevice()
{

}
