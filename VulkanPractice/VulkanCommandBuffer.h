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

	VkCommandBuffer
	getHandle();

private:
	VkCommandBuffer m_commandBuffer;
};