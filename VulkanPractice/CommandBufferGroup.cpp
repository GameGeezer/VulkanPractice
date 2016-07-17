#include "CommandBufferGroup.h"

#include "CommandPool.h"

CommandBufferGroup::CommandBufferGroup(VkDevice device, CommandPool& commandPool, uint32_t buffersToAllocate, VkCommandBufferLevel level) : m_device(device), count(buffersToAllocate), m_commandBuffers(new VkCommandBuffer[buffersToAllocate])
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandBufferCount = buffersToAllocate;
	commandBufferAllocateInfo.commandPool = commandPool.getHandle();
	commandBufferAllocateInfo.level = level;

	vkAllocateCommandBuffers(m_device, &commandBufferAllocateInfo, m_commandBuffers);
}

CommandBufferGroup::~CommandBufferGroup()
{

}

VkCommandBuffer
CommandBufferGroup::getCommandBufferAtIndex(uint32_t index)
{
	return m_commandBuffers[index];
}