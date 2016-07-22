#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanCommandPool;
class VulkanCommandBuffer;

class VulkanCommandBufferGroup
{
public:
	const uint32_t count;

	VulkanCommandBufferGroup(VkDevice device, VkCommandPool commandPool, uint32_t buffersToAllocate, VkCommandBufferLevel level);

	~VulkanCommandBufferGroup();

	VulkanCommandBuffer*
	getCommandBufferAtIndex(uint32_t index);

private:
	VkDevice			m_device;
	VkCommandBuffer*	m_commandBuffers;

	std::vector<VulkanCommandBuffer*> m_commandBufferWrappers;
};