#include "VulkanCommandBuffer.h"

VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer commandBuffer) : m_commandBuffer(commandBuffer)
{

}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
	
}

void
VulkanCommandBuffer::begin(VkCommandBufferUsageFlagBits flags)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = flags;

	vkBeginCommandBuffer(m_commandBuffer, &beginInfo);
}

void
VulkanCommandBuffer::end()
{
	vkEndCommandBuffer(m_commandBuffer);
}

VkCommandBuffer
VulkanCommandBuffer::getHandle()
{
	return m_commandBuffer;
}