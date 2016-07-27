#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanCommandBuffer;

class VulkanCommandBufferGroup
{
public:
	const uint32_t count;

	VulkanCommandBufferGroup(VkDevice device, VkCommandPool commandPool, uint32_t buffersToAllocate, VkCommandBufferLevel level);

	~VulkanCommandBufferGroup();

	VulkanCommandBuffer*
	getCommandBufferAtIndex(uint32_t index);

	void
	freeBuffers();

private:
	VkDevice				m_device;
	VkCommandBuffer		*	m_commandBuffers;
	VkCommandPool			m_commandPool;

	std::vector<VulkanCommandBuffer*> m_commandBufferWrappers;
};