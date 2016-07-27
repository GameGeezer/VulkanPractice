#pragma once

#include <vulkan\vulkan.h>

class VulkanDevice;
class VulkanBuffer;
class VulkanDeviceMemory;

class VulkanUniformBuffer
{
public:
	VulkanUniformBuffer(VulkanDevice &device, uint32_t sizeInBytes);

	~VulkanUniformBuffer();

	void
	bind();

	void
	update(void *start, size_t size);

private:
	VulkanDevice		*	m_device;

	VulkanBuffer		*	m_stagingBuffer;
	VulkanDeviceMemory	*	m_stagingMemory;

	VulkanBuffer		*	m_buffer;
	VulkanDeviceMemory	*	m_memory;

	VkDescriptorBufferInfo	m_descriptor;
};