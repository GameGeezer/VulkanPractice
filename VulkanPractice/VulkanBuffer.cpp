#include "VulkanBuffer.h"

#include "Shared.h"

#include "VulkanDevice.h"

#include "VulkanCommandBuffer.h"

#include "VulkanDeviceMemory.h"

VulkanBuffer::VulkanBuffer(VulkanDevice &device, uint32_t sizeBytes, uint32_t usage) : m_device(&device)
{
	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.size = sizeBytes;
	bufferCreateInfo.usage = usage;

	ErrorCheck(vkCreateBuffer(m_device->getHandle(), &bufferCreateInfo, nullptr, &m_buffer));

	vkGetBufferMemoryRequirements(m_device->getHandle(), m_buffer, &m_bufferMemoryRequirements);
}

VulkanBuffer::~VulkanBuffer()
{
	vkDestroyBuffer(m_device->getHandle(), m_buffer, nullptr);
}

void
VulkanBuffer::copyInto(VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset, VkBuffer dstBuffer, VulkanCommandBuffer &commandBuffer)
{
	//	Define the region of the buffer to copy and where to place
	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	copyRegion.srcOffset = srcOffset;
	copyRegion.dstOffset = dstOffset;

	// Start recording commands
	commandBuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	// Add a copy buffer command
	vkCmdCopyBuffer(commandBuffer.getHandle(), m_buffer, dstBuffer, 1, &copyRegion);

	// Stop recording commands
	commandBuffer.end();

	VkCommandBuffer rawCommandBuffer = commandBuffer.getHandle();

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &rawCommandBuffer;

	m_device->submitToQueue(1, &submitInfo, VK_NULL_HANDLE);

	m_device->waitOnQueue();
}

void
VulkanBuffer::setMemory(void *data, uint32_t offset, size_t size)
{
	VulkanDeviceMemory memoryWrapper(m_device->getHandle(), m_memoryBoundTo);
	void *pData = nullptr;
	void *mapping = memoryWrapper.map(0, VK_WHOLE_SIZE);
	memoryWrapper.copyFrom(mapping, data, offset, size);
	memoryWrapper.unmap();
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
	m_memoryBoundTo = deviceMemory;

	vkBindBufferMemory(m_device->getHandle(), m_buffer, deviceMemory, offset);
}
