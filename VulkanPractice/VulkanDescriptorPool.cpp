#include "VulkanDescriptorPool.h"

#include "Shared.h"

VulkanDescriptorPool::VulkanDescriptorPool()
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
VulkanDescriptorPool::addSize(uint32_t descriptorCount)
{
	VkDescriptorPoolSize poolSize = {};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = descriptorCount;

	m_poolSizes.push_back(poolSize);
}