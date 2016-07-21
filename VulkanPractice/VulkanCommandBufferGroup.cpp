#include "VulkanCommandBufferGroup.h"

#include "VulkanCommandBuffer.h"

VulkanCommandBufferGroup::VulkanCommandBufferGroup(VkDevice device, VkCommandPool commandPool, uint32_t buffersToAllocate, VkCommandBufferLevel level) : m_device(device), count(buffersToAllocate), m_commandBuffers(new VkCommandBuffer[buffersToAllocate])
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandBufferCount = buffersToAllocate;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = level;

	vkAllocateCommandBuffers(m_device, &commandBufferAllocateInfo, m_commandBuffers);

	for (uint32_t i = 0; i < buffersToAllocate; ++i)
	{
		m_commandBufferWrappers.push_back(new VulkanCommandBuffer(m_commandBuffers[i]));
	}
}

VulkanCommandBufferGroup::~VulkanCommandBufferGroup()
{

}

VulkanCommandBuffer*
VulkanCommandBufferGroup::getCommandBufferAtIndex(uint32_t index)
{
	return m_commandBufferWrappers.at(index);
}