#include "VulkanEnumeratedPhysicalDevices.h"

#include "VulkanPhysicalDevice.h"

VulkanEnumeratedPhysicalDevices::VulkanEnumeratedPhysicalDevices(VkInstance instance)
{
	// Count the number of physical devices
	vkEnumeratePhysicalDevices(instance, &m_physicalDeviceCount, nullptr);
	// Find handles for the physical devices
	std::vector<VkPhysicalDevice> rawPhysicalDevices(m_physicalDeviceCount);
	vkEnumeratePhysicalDevices(instance, &m_physicalDeviceCount, rawPhysicalDevices.data());

	for (auto physicalDevice : rawPhysicalDevices)
	{
		m_physicalDevices.push_back(new VulkanPhysicalDevice(physicalDevice));
	}
}

VulkanEnumeratedPhysicalDevices::~VulkanEnumeratedPhysicalDevices()
{

}

void
VulkanEnumeratedPhysicalDevices::findDevicesWithQueueFlag(VkQueueFlagBits flagBit, std::vector<VulkanPhysicalDevice*> &out_devices, std::vector<uint32_t> &out_indexes)
{
	for (auto physicalDevice : m_physicalDevices)
	{
		uint32_t queueIndex = -1;
		
		if (physicalDevice->hasQueueFamilyWithFlag(flagBit, queueIndex))
		{
			out_devices.push_back(physicalDevice);

			out_indexes.push_back(queueIndex);
		}
	}
}