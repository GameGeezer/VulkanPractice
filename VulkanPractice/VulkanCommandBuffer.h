#pragma once

#include <vulkan\vulkan.h>

class VulkanCommandBuffer
{
public:
	VulkanCommandBuffer(VkCommandBuffer commandBuffer);

	~VulkanCommandBuffer();

	void
	begin(VkCommandBufferUsageFlags flags);

	void
	end();

	void
	commandCopyBuffer(VkBuffer srcBuffer, VkDeviceSize srcOffset, VkBuffer dstBuffer, VkDeviceSize dstOffset, VkDeviceSize size);

	void
	commandCopyImage2D(uint32_t width, uint32_t height, VkImage srcImage, int32_t srcX, int32_t srcY, VkImage dstImage, int32_t dstX, int32_t dstY);

	void
	commandCopyImage3D(uint32_t width, uint32_t height, uint32_t depth, VkImage srcImage, int32_t srcX, int32_t srcY, int32_t srcZ, VkImage dstImage, int32_t dstX, int32_t dstY, int32_t dstZ);

	void
	addBarriers(VkPipelineStageFlags srcFlags, VkPipelineStageFlags dstFlags, VkDependencyFlags depFlags, VkMemoryBarrier *memoryBarriers, uint32_t memoryBarriersCount, VkBufferMemoryBarrier *bufferMemoryBarriers, uint32_t bufferMemoryBarriersCount,VkImageMemoryBarrier *imageBarriers, uint32_t imageBarriersCount);

	VkCommandBuffer
	getHandle();

private:
	VkCommandBuffer m_commandBuffer;
};