#include "VulkanPhysicalDevice.h"

VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice physicalDevice) : m_physicalDevice(physicalDevice)
{
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &m_queueFamilyPropertyCount, nullptr);

	m_queueFamilyProperties.resize(m_queueFamilyPropertyCount);

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &m_queueFamilyPropertyCount, m_queueFamilyProperties.data());

	vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);

	vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_physicalDeviceFeatures);

	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_physicalDeviceMemoryProperties);
}

VulkanPhysicalDevice::~VulkanPhysicalDevice()
{
	
}

bool
VulkanPhysicalDevice::hasQueueFamilyWithFlag(VkQueueFlagBits flagBit, uint32_t &out_queueIndex)
{
	uint32_t i = 0;
	for (const auto& queueFamilyProperty : m_queueFamilyProperties)
	{
		if (queueFamilyProperty.queueFlags & flagBit)
		{
			out_queueIndex = i;

			return true;
		}

		++i;
	}

	return false;
}

VkPhysicalDevice
VulkanPhysicalDevice::getHandle()
{
	return m_physicalDevice;
}

VkPhysicalDeviceProperties*
VulkanPhysicalDevice::getProperties()
{
	return &m_physicalDeviceProperties;
}

VkPhysicalDeviceMemoryProperties*
VulkanPhysicalDevice::getMemoryProperties()
{
	return &m_physicalDeviceMemoryProperties;
}

VkPhysicalDeviceFeatures*
VulkanPhysicalDevice::getFeatures()
{
	return &m_physicalDeviceFeatures;
}