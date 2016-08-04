#include "VulkanCommandBuffer.h"


VulkanCommandBuffer::VulkanCommandBuffer(VkCommandBuffer commandBuffer) : m_commandBuffer(commandBuffer)
{

}

VulkanCommandBuffer::~VulkanCommandBuffer()
{
	
}

void
VulkanCommandBuffer::begin(VkCommandBufferUsageFlags flags)
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
VulkanCommandBuffer::commandCopyBuffer(VkBuffer srcBuffer, VkDeviceSize srcOffset, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size)
{
	VkBufferCopy copyRegion = {};
	copyRegion.size = size;
	copyRegion.srcOffset = srcOffset;
	copyRegion.dstOffset = dstOffset;

	vkCmdCopyBuffer(m_commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
}
void
VulkanCommandBuffer::commandCopyImage2D(uint32_t width, uint32_t height, VkImage srcImage, int32_t srcX, int32_t srcY, VkImage dstImage, int32_t dstX, int32_t dstY)
{
	commandCopyImage3D(width, height, 1, srcImage, srcX, srcY, 0, dstImage, dstX, dstY, 0);
}

void
VulkanCommandBuffer::commandCopyImage3D(uint32_t width, uint32_t height, uint32_t depth, VkImage srcImage, int32_t srcX, int32_t srcY, int32_t srcZ, VkImage dstImage, int32_t dstX, int32_t dstY, int32_t dstZ)
{
	VkImageSubresourceLayers subResource = {};
	subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	subResource.baseArrayLayer = 0;
	subResource.mipLevel = 0;
	subResource.layerCount = 1;

	VkImageCopy region = {};
	region.srcSubresource = subResource;
	region.dstSubresource = subResource;
	region.srcOffset = { srcX, srcY, srcZ };
	region.dstOffset = { dstX, dstY, dstZ };
	region.extent.width = width;
	region.extent.height = height;
	region.extent.depth = depth;

	vkCmdCopyImage(m_commandBuffer, srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
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