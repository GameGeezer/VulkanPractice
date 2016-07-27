#pragma once

#include <vulkan\vulkan.h>

class VulkanDevice;

class VulkanBuffer
{
public:
	VulkanBuffer(VulkanDevice &device, uint32_t sizeBytes, uint32_t usage);

	~VulkanBuffer();


	VkBuffer
	getHandle();

	uint32_t
	getMemoryTypeBits();

	VkDeviceSize
	getSize();

	VkDeviceSize
	getAlignment();

	void
	bindToMemory(VkDeviceMemory deviceMemory, uint32_t offset);

private:
	VulkanDevice			*	m_device;
	VkBuffer					m_buffer;
	VkMemoryRequirements		m_bufferMemoryRequirements = {};
};