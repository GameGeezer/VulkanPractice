#pragma once

#include <vulkan\vulkan.h>

class VulkanDeviceMemory
{
public:
	VulkanDeviceMemory(VkDevice device, VkDeviceMemory deviceMemory);

	~VulkanDeviceMemory();

	void*
	map(VkDeviceSize offset, VkDeviceSize size);

	void
	unmap();

	void
	copyInto(const void *data, uint32_t offset, size_t size);

	VkDeviceMemory
	getHandle();

private:
	VkDevice		m_device;
	VkDeviceMemory	m_deviceMemory;

	void*			m_mapping = nullptr;
};