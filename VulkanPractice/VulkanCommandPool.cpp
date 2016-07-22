#include "VulkanCommandPool.h"

VulkanCommandPool::VulkanCommandPool(VkDevice device, bool transient, bool individual, uint32_t queueFamilyIndex) : m_device(device)
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
	commandPoolCreateInfo.flags = (VK_COMMAND_POOL_CREATE_TRANSIENT_BIT * transient) & (VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT * individual);

	vkCreateCommandPool(m_device, &commandPoolCreateInfo, nullptr, &m_commandPool);
}

VulkanCommandPool::~VulkanCommandPool()
{
	vkDestroyCommandPool(m_device, m_commandPool, nullptr);
}

void
VulkanCommandPool::reset(bool releaseResources)
{
	vkResetCommandPool(m_device, m_commandPool, VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT * releaseResources);
}

VkCommandPool
VulkanCommandPool::getHandle()
{
	return m_commandPool;
}