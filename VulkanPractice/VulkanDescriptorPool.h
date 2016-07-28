#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanDescriptorPool
{
public:
	VulkanDescriptorPool(VkDevice device);

	~VulkanDescriptorPool();

	void 
	initialize(uint32_t maxSets);

	void
	addSize(uint32_t descriptorCount);

	VkDescriptorPool
	getHandle();

private:
	VkDevice							m_device;
	VkDescriptorPool					m_descriptorPool;
	std::vector<VkDescriptorPoolSize>	m_poolSizes;
	bool								m_initialized = false;
};