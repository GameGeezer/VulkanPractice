#include "VulkanMemoryAllocateInfo.h"

VulkanMemoryAllocateInfo::VulkanMemoryAllocateInfo(VkDeviceSize allocationSize, uint32_t memoryTypeIndex)
{
	m_memoryAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	m_memoryAllocInfo.pNext = NULL;
	m_memoryAllocInfo.allocationSize = allocationSize;
	m_memoryAllocInfo.memoryTypeIndex = memoryTypeIndex;
}

VulkanMemoryAllocateInfo::~VulkanMemoryAllocateInfo()
{

}

VkMemoryAllocateInfo*
VulkanMemoryAllocateInfo::getAllocateInfo()
{
	return &m_memoryAllocInfo;
}
