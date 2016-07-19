#pragma once

#include <vulkan\vulkan.h>

class VulkanMemoryAllocateInfo
{
public:
	VulkanMemoryAllocateInfo(VkDeviceSize allocationSize, uint32_t memoryTypeIndex);

	~VulkanMemoryAllocateInfo();

	VkMemoryAllocateInfo*
	getAllocateInfo();

private:
	VkMemoryAllocateInfo m_memoryAllocInfo = {};
};