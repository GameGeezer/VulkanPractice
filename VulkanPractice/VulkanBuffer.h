#pragma once

#include <vulkan\vulkan.h>

class VulkanBuffer
{
public:
	VulkanBuffer(VkDevice device, uint32_t sizeBytes, VkBufferUsageFlagBits usage);

	~VulkanBuffer();

	VkBuffer
	getHandle();

	VkDeviceSize
	getSize();

	VkDeviceSize
	getAlignment();

	void
	bindToMemory(VkDeviceMemory deviceMemory, uint32_t offset);

private:
	VkDevice m_device;
	VkBuffer m_buffer;

	VkMemoryRequirements m_bufferMemoryRequirements = {};
};
