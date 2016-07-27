#include "VulkanBuffer.h"

#include "Shared.h"

#include "VulkanDevice.h"

VulkanBuffer::VulkanBuffer(VulkanDevice &device, uint32_t sizeBytes, uint32_t usage) : m_device(&device)
{
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = sizeBytes;
	bufferCreateInfo.usage = usage;

	ErrorCheck(vkCreateBuffer(m_device->getDevice(), &bufferCreateInfo, nullptr, &m_buffer));

	vkGetBufferMemoryRequirements(m_device->getDevice(), m_buffer, &m_bufferMemoryRequirements);
}

VulkanBuffer::~VulkanBuffer()
{
	vkDestroyBuffer(m_device->getDevice(), m_buffer, nullptr);
}


VkBuffer
VulkanBuffer::getHandle()
{
	return m_buffer;
}

uint32_t
VulkanBuffer::getMemoryTypeBits()
{
	return m_bufferMemoryRequirements.memoryTypeBits;
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
	vkBindBufferMemory(m_device->getDevice(), m_buffer, deviceMemory, offset);
}
