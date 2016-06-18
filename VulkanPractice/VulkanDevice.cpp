#include "VulkanDevice.h"

VulkanDevice::VulkanDevice(VkDevice device, VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties physicalDeviceProperties, uint32_t queueFamilyCount, VkQueueFamilyProperties queueFamilyProperties) : device(device), physicalDevice(physicalDevice), physicalDeviceProperties(physicalDeviceProperties), queueFamilyCount(queueFamilyCount), queueFamilyProperties(queueFamilyProperties)
{
	
}

VulkanDevice::~VulkanDevice()
{

}

void VulkanDevice::getQueue(uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* queue)
{
	queueFamilyProperties.
	for (uint32_t i = 0; i < queueFamilyCount; ++i) {
		if (queueFamilyProperties[i].queueFlags & queueFamilyIndex) {
			found = true;
			_graphics_family_index = i;
		}
	}

	vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, queue);
}