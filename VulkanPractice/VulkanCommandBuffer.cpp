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

void
VulkanCommandBuffer::addBarriers(VkPipelineStageFlags srcFlags, VkPipelineStageFlags dstFlags, VkDependencyFlags depFlags, VkMemoryBarrier *memoryBarriers, uint32_t memoryBarriersCount, VkBufferMemoryBarrier *bufferMemoryBarriers, uint32_t bufferMemoryBarriersCount, VkImageMemoryBarrier *imageBarriers, uint32_t imageBarriersCount)
{
	vkCmdPipelineBarrier(m_commandBuffer, srcFlags, dstFlags, dstFlags, memoryBarriersCount, memoryBarriers, bufferMemoryBarriersCount, bufferMemoryBarriers, imageBarriersCount, imageBarriers);
}

VkCommandBuffer
VulkanCommandBuffer::getHandle()
{
	return m_commandBuffer;
}