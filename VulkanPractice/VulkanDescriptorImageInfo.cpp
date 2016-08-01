#include "VulkanDescriptorImageInfo.h"

VulkanDescriptorImageInfo::VulkanDescriptorImageInfo(VkImageLayout imageLayout, VkImageView imageView, VkSampler sampler)
{
	m_descriptorImageInfo.imageLayout = imageLayout;
	m_descriptorImageInfo.imageView = imageView;
	m_descriptorImageInfo.sampler = sampler;
}

VulkanDescriptorImageInfo::~VulkanDescriptorImageInfo()
{

}

VkDescriptorImageInfo*
VulkanDescriptorImageInfo::getRaw()
{
	return &m_descriptorImageInfo;
}