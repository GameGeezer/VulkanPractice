#pragma once

#include <vulkan\vulkan.h>

class VulkanDescriptorImageInfo
{
public:
	VulkanDescriptorImageInfo(VkImageLayout imageLayout, VkImageView imageView, VkSampler sampler);

	~VulkanDescriptorImageInfo();

	VkDescriptorImageInfo*
	getRaw();

private:
	VkDescriptorImageInfo	m_descriptorImageInfo = {};
};