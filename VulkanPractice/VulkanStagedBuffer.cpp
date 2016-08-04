#include "VulkanStagedBuffer.h"


#include "Shared.h"

#include "VulkanDevice.h"

#include "VulkanBuffer.h"
#include "VulkanDeviceMemory.h"
#include "VulkanCommandBuffer.h"

VulkanStagedBuffer::VulkanStagedBuffer(VulkanDevice &device, uint32_t sizeInBytes, VkBufferUsageFlagBits usage) : m_device(&device)
{
	m_stagingBuffer = new VulkanBuffer(device, sizeInBytes, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VkSharingMode::VK_SHARING_MODE_EXCLUSIVE);
	VkDeviceMemory memory = m_device->allocateMemory(m_stagingBuffer->getMemoryTypeBits(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeInBytes);
	m_stagingMemory = new VulkanDeviceMemory(m_device->getHandle(), memory);

	m_buffer = new VulkanBuffer(device, sizeInBytes, VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage, VkSharingMode::VK_SHARING_MODE_EXCLUSIVE);
	memory = m_device->allocateMemory(m_buffer->getMemoryTypeBits(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sizeInBytes);
	m_memory = new VulkanDeviceMemory(m_device->getHandle(), memory);

	m_stagingBuffer->bindToMemory(m_stagingMemory->getHandle(), 0);
	m_buffer->bindToMemory(m_memory->getHandle(), 0);
}

VulkanStagedBuffer::~VulkanStagedBuffer()
{

}

void
VulkanStagedBuffer::stage(void *data, uint32_t offset, size_t size)
{
	void *pData = nullptr;
	void *mapping = m_stagingMemory->map(0, VK_WHOLE_SIZE);
	m_stagingMemory->copyFrom(mapping, data, offset, size);
	m_stagingMemory->unmap();
}

void
VulkanStagedBuffer::update(VulkanCommandBuffer &commandBuffer)
{
	commandBuffer.begin(0);

	commandBuffer.commandCopyBuffer(m_stagingBuffer->getHandle(), 0, m_buffer->getHandle(), 0, m_buffer->getSize());

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
VulkanStagedBuffer::getBuffer()
{
	return m_buffer->getHandle();
}