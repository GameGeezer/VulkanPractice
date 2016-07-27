#include "VulkanUniformBuffer.h"

#include "Shared.h"

#include "VulkanDevice.h"

#include "VulkanBuffer.h"
#include "VulkanDeviceMemory.h"

VulkanUniformBuffer::VulkanUniformBuffer(VulkanDevice &device, uint32_t sizeInBytes) : m_device(&device)
{
	m_stagingBuffer = new VulkanBuffer(device, sizeInBytes, VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	VkDeviceMemory memory = m_device->allocateMemory(m_buffer->getMemoryTypeBits(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sizeInBytes);
	m_stagingMemory = new VulkanDeviceMemory(m_device->getDevice(), memory);

	m_stagingBuffer = new VulkanBuffer(device, sizeInBytes, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
	memory = m_device->allocateMemory(m_buffer->getMemoryTypeBits(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sizeInBytes);
	m_memory = new VulkanDeviceMemory(m_device->getDevice(), memory);

}

VulkanUniformBuffer::~VulkanUniformBuffer()
{

}

void
VulkanUniformBuffer::bind()
{
	//ErrorCheck(vkBindBufferMemory(m_device->getDevice(), m_buffer, m_memory, 0));
}

void
VulkanUniformBuffer::update(void *start, size_t size)
{
	void *pData = nullptr;
	//ErrorCheck(vkMapMemory(m_device->getDevice(), m_memory, 0, size, 0, &start));
	//memcpy(pData, start, size);
	//(m_device->getDevice(), m_memory);

	m_stagingMemory->map(0, VK_WHOLE_SIZE);
	memcpy(pData, start, size);
	m_stagingMemory->unmap();
}