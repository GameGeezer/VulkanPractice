#pragma once

#include <vulkan\vulkan.h>

class VulkanDevice;
class VulkanBuffer;
class VulkanDeviceMemory;
class VulkanCommandBuffer;

class VulkanStagedBuffer
{
public:
	VulkanStagedBuffer(VulkanDevice &device, uint32_t sizeInBytes, VkBufferUsageFlagBits usage);

	~VulkanStagedBuffer();

	void
	update(void *start, size_t size, VulkanCommandBuffer &commandBuffer);

	VkBuffer
	getBuffer();

private:
	VulkanDevice		*	m_device;

	VulkanBuffer		*	m_stagingBuffer;
	VulkanDeviceMemory	*	m_stagingMemory;

	VulkanBuffer		*	m_buffer;
	VulkanDeviceMemory	*	m_memory;

	VkDescriptorBufferInfo	m_descriptor;
};