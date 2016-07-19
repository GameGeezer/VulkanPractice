#include "VulkanBuffer.h"

#include "Shared.h"

#include "VulkanMemoryAllocateInfo.h"

VulkanBuffer::VulkanBuffer(VkDevice device, uint32_t sizeBytes, VkBufferUsageFlagBits usage) : m_device(device)
{
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = sizeBytes;
	bufferCreateInfo.usage = usage;

	ErrorCheck(vkCreateBuffer(device, &bufferCreateInfo, nullptr, &m_buffer));

	vkGetBufferMemoryRequirements(m_device, m_buffer, &m_bufferMemoryRequirements);
}

VulkanBuffer::~VulkanBuffer()
{
	vkDestroyBuffer(m_device, m_buffer, nullptr);
}

VkBuffer
VulkanBuffer::getHandle()
{
	return m_buffer;
}

VkDeviceSize
VulkanBuffer::getSize()
{
	return m_bufferMemoryRequirements.size;
}

VkDeviceSize
VulkanBuffer::getAlignment()
{
	return m_bufferMemoryRequirements.alignment;
}

void
VulkanBuffer::bindToMemory(VkDeviceMemory deviceMemory, uint32_t offset)
{
	vkBindBufferMemory(m_device, m_buffer, deviceMemory, offset);
}
