#include "VulkanDescriptorPool.h"

#include "Shared.h"

VulkanDescriptorPool::VulkanDescriptorPool(VkDevice device) : m_device(device)
{

}

VulkanDescriptorPool::~VulkanDescriptorPool()
{

}

void
VulkanDescriptorPool::initialize(uint32_t maxSets)
{
	if (m_initialized)
	{
		throw "VulkanDescriptorPool has already been initialized";
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(m_poolSizes.size());
	poolInfo.pPoolSizes = m_poolSizes.data();
	poolInfo.maxSets = maxSets;

	ErrorCheck(vkCreateDescriptorPool(m_device, &poolInfo, nullptr, &m_descriptorPool));

	m_initialized = true;
}

void
VulkanDescriptorPool::addSize(VkDescriptorType type, uint32_t count)
{
	VkDescriptorPoolSize poolSize = {};
	poolSize.type = type;
	poolSize.descriptorCount = type;

	m_poolSizes.push_back(poolSize);
}


VkDescriptorPool
VulkanDescriptorPool::getHandle()
{
	return m_descriptorPool;
}