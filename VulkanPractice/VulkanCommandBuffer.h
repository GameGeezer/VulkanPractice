#pragma once

#include <vulkan\vulkan.h>

class VulkanCommandBuffer
{
public:
	VulkanCommandBuffer(VkCommandBuffer commandBuffer);

	~VulkanCommandBuffer();

	void
	begin(VkCommandBufferUsageFlagBits flags);

	void
	end();

	void
	addBarriers(VkPipelineStageFlags srcFlags, VkPipelineStageFlags dstFlags, VkDependencyFlags depFlags, VkMemoryBarrier *memoryBarriers, uint32_t memoryBarriersCount, VkBufferMemoryBarrier *bufferMemoryBarriers, uint32_t bufferMemoryBarriersCount,VkImageMemoryBarrier *imageBarriers, uint32_t imageBarriersCount);

	VkCommandBuffer
	getHandle();

private:
	VkCommandBuffer m_commandBuffer;
};