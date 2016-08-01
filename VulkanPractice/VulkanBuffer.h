#pragma once

#include <vulkan\vulkan.h>

class VulkanDevice;
class VulkanCommandBuffer;

class VulkanBuffer
{
public:
	VulkanBuffer(VulkanDevice &device, uint32_t sizeBytes, uint32_t usage);

	~VulkanBuffer();

	//	size - The ammount of memory in byes inwhich to copy
	//	srcOffset - The offset into the source buffer that the copy should start at
	//	dstOffset - The offset into the destination buffer that the copy should start at
	//	commandBuffer - The command buffer that the copy command will be added to
	//	post - dst buffer now contains the coppied data
	void
	copyInto(VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkBuffer dstBuffer, VulkanCommandBuffer &commandBuffer);

	void
	setMemory(void *data, uint32_t offset, size_t size);

	// returns - A pointer to the buffer
	VkBuffer
	getHandle();

	// returns - Each bit itentifies a differnt memory type that is supported
	uint32_t
	getMemoryTypeBits();

	//	returns - Memory size ine bytes
	VkDeviceSize
	getSize();

	//	returns -The alignment of the offset within the allocation in bytes
	VkDeviceSize
	getAlignment();

	//	deviceMemory - The memory to bind to
	//	offset - The offset into the memory inwhich to bind
	//	post - This buffer is bound to a block of device memory at offset
	void
	bindToMemory(VkDeviceMemory deviceMemory, uint32_t offset);

private:
	VulkanDevice			*	m_device;
	VkBuffer					m_buffer;
	VkMemoryRequirements		m_bufferMemoryRequirements = {};

	//	When bound
	VkDeviceMemory				m_memoryBoundTo;
};