#include "VulkanDeviceMemory.h"

#include <vector>

VulkanDeviceMemory::VulkanDeviceMemory(VkDevice device, VkDeviceMemory deviceMemory) : m_device(device), m_deviceMemory(deviceMemory)
{

}

VulkanDeviceMemory::~VulkanDeviceMemory()
{

}

void*
VulkanDeviceMemory::map(VkDeviceSize offset, VkDeviceSize size)
{
	vkMapMemory(m_device, m_deviceMemory, 0, size, 0, &m_mapping);

	return m_mapping;
}

void
VulkanDeviceMemory::unmap()
{
	vkUnmapMemory(m_device, m_deviceMemory);

	m_mapping = nullptr;
}

void
VulkanDeviceMemory::copyInto(const void *data, uint32_t offset, size_t size)
{
	::memcpy(static_cast<uint8_t*> (m_mapping) + offset, data, size);
}