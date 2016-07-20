#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class CommandPool;
class VulkanCommandBuffer;

class CommandBufferGroup
{
public:
	const uint32_t count;

	CommandBufferGroup(VkDevice device, CommandPool& commandPool, uint32_t buffersToAllocate, VkCommandBufferLevel level);

	~CommandBufferGroup();

	VulkanCommandBuffer*
	getCommandBufferAtIndex(uint32_t index);

private:
	VkDevice			m_device;
	VkCommandBuffer*	m_commandBuffers;

	std::vector<VulkanCommandBuffer*> m_commandBufferWrappers;
};