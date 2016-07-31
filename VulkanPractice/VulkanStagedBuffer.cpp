#include "VulkanStagedBuffer.h"


#include "Shared.h"

#include "VulkanDevice.h"

#include "VulkanBuffer.h"
#include "VulkanDeviceMemory.h"
#include "VulkanCommandBuffer.h"

VulkanStagedBuffer::VulkanStagedBuffer(VulkanDevice &device, uint32_t sizeInBytes, VkBufferUsageFlagBits usage) : m_device(&device)
{
	m_stagingBuffer = new VulkanBuffer(device, sizeInBytes, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	VkDeviceMemory memory = m_device->allocateMemory(m_stagingBuffer->getMemoryTypeBits(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeInBytes);
	m_stagingMemory = new VulkanDeviceMemory(m_device->getDevice(), memory);

	m_buffer = new VulkanBuffer(device, sizeInBytes, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage);
	memory = m_device->allocateMemory(m_buffer->getMemoryTypeBits(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sizeInBytes);
	m_memory = new VulkanDeviceMemory(m_device->getDevice(), memory);

	m_stagingBuffer->bindToMemory(m_stagingMemory->getHandle(), 0);
	m_buffer->bindToMemory(m_memory->getHandle(), 0);
}

VulkanStagedBuffer::~VulkanStagedBuffer()
{

}

void
VulkanStagedBuffer::stage(void *start, size_t size)
{
	void *pData = nullptr;
	m_stagingMemory->map(0, VK_WHOLE_SIZE);
	m_stagingMemory->copyInto(start, 0, size);
	m_stagingMemory->unmap();
}

void
VulkanStagedBuffer::update(VulkanCommandBuffer &commandBuffer)
{
	m_stagingBuffer->copyInto(m_buffer->getHandle(), m_buffer->getSize(), commandBuffer);
}


VkBuffer
VulkanStagedBuffer::getBuffer()
{
	return m_buffer->getHandle();
}