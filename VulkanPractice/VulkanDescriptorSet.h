#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanDescriptorSet
{
public: 
	VulkanDescriptorSet(VkDevice device);

	~VulkanDescriptorSet();

	void
	initialize(VkDescriptorPool pool, uint32_t numberToAllocate);

	void
	addLayout(VkDescriptorSetLayout layout);

	uint32_t
	getCount();

	VkDescriptorSet
	getHandle();

private:
	VkDevice								m_device;
	VkDescriptorSet							m_descriptorSet;
	std::vector<VkDescriptorSetLayout>		m_layouts;
	uint32_t								m_count;
	bool									m_initailized = false;
};
