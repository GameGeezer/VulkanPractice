#include "VulkanFenceGroup.h"

#include "Shared.h"

VulkanFenceGroup::VulkanFenceGroup(VkDevice device, uint32_t count) : m_device(device), m_count(count), m_fences(new VkFence[count])
{
	
}

VulkanFenceGroup::~VulkanFenceGroup()
{
	delete[] m_fences;
}

VkFence
VulkanFenceGroup::getFenceAtIndex(uint32_t index)
{
	return m_fences[index];
}

VkFence*
VulkanFenceGroup::getPointerToFenceAtIndex(uint32_t index)
{
	return &m_fences[index];
}

void
VulkanFenceGroup::initFenceAtIndex(uint32_t index, bool signaled)
{
	VkFenceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	createInfo.pNext = NULL;
	createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT * signaled;	// 0 if not signaled, 1 if signaled

	ErrorCheck(vkCreateFence(m_device, &createInfo, nullptr, m_fences + index));
}

void
VulkanFenceGroup::destroyFenceAtIndex(uint32_t index)
{
	vkDestroyFence(m_device, *(m_fences + index), nullptr);
}

bool
VulkanFenceGroup::getFenceStatusAtIndex(uint32_t index)
{
	return vkGetFenceStatus(m_device, *(m_fences + index)) == VK_SUCCESS;
}

void
VulkanFenceGroup::resetFences(uint32_t startIndex, uint32_t count)
{
	vkResetFences(m_device, count, m_fences + startIndex);
}

void
VulkanFenceGroup::waitForFences(uint32_t startIndex, uint32_t count, VkBool32 waitAll, uint64_t timeout)
{
	vkWaitForFences(m_device, count, m_fences + startIndex, waitAll, timeout);
}