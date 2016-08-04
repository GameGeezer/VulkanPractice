#include "VulkanDeviceMemory.h"

#include <vector>

VulkanDeviceMemory::VulkanDeviceMemory(VkDevice device, VkDeviceMemory deviceMemory) : m_device(device), m_deviceMemory(deviceMemory)
{

}

VulkanDeviceMemory::~VulkanDeviceMemory()
{
	vkFreeMemory(m_device, m_deviceMemory, nullptr);
}

void*
VulkanDeviceMemory::map(VkDeviceSize offset, VkDeviceSize size)
{
	void *mapping = nullptr;
	//	Map size ammount of memory starting at offset
	vkMapMemory(m_device, m_deviceMemory, offset, size, 0, &mapping);
	//	Return the mapping
	return mapping;
}

void
VulkanDeviceMemory::unmap()
{
	vkUnmapMemory(m_device, m_deviceMemory);
}

void
VulkanDeviceMemory::copyFrom(void *mapping, const void *data, uint32_t offset, size_t size)
{
	::memcpy(static_cast<uint8_t*> (mapping) + offset, data, size);
}

VkDeviceMemory
VulkanDeviceMemory::getHandle()
{
	return m_deviceMemory;
}