#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanDescriptorSetGroup
{
public: 
	VulkanDescriptorSetGroup(VkDevice device);

	~VulkanDescriptorSetGroup();

	void
	initialize(VkDescriptorPool pool, uint32_t numberToAllocate);

	void
	addLayout(VkDescriptorSetLayout layout);

	uint32_t
	getCount();

	VkDescriptorSet
	getDescriptorSetAtIndex(uint32_t index);

private:
	VkDevice								m_device;
	VkDescriptorSet						*	m_descriptorSets;
	std::vector<VkDescriptorSetLayout>		m_layouts;
	uint32_t								m_count;
	bool									m_initailized = false;
};
