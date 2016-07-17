#pragma once

#include <vulkan\vulkan.h>

class CommandPool;

class CommandBufferGroup
{
public:
	const uint32_t count;

	CommandBufferGroup(VkDevice device, CommandPool& commandPool, uint32_t buffersToAllocate, VkCommandBufferLevel level);

	~CommandBufferGroup();

	VkCommandBuffer
	getCommandBufferAtIndex(uint32_t index);

private:
	VkDevice			m_device;
	VkCommandBuffer*	m_commandBuffers;
};