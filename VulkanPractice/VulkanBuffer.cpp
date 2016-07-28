#include "VulkanBuffer.h"

#include "Shared.h"

#include "VulkanDevice.h"

#include "VulkanCommandBuffer.h"

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

void
VulkanBuffer::copyInto(VkBuffer dstBuffer, uint64_t size, VulkanCommandBuffer &commandBuffer)
{
	commandBuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer.getHandle(), m_buffer, dstBuffer, 1, &copyRegion);

	commandBuffer.end();

	VkCommandBuffer rawCommandBuffer = commandBuffer.getHandle();
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &rawCommandBuffer;

	m_device->submitToQueue(1, &submitInfo, VK_NULL_HANDLE);

	m_device->waitOnQueue();
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
