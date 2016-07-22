#include "VulkanSemaphore.h"

VulkanSemaphore::VulkanSemaphore(VkDevice device) : m_device(device)
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_semaphore);
}

VulkanSemaphore::~VulkanSemaphore()
{
	vkDestroySemaphore(m_device, m_semaphore, nullptr);
}

VkSemaphore
VulkanSemaphore::getHandle()
{
	return m_semaphore;
}