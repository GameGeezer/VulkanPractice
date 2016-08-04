#include "VulkanTexture2D.h"

#include "VulkanImageView.h"
#include "VulkanSampler.h"

VulkanTexture2D::VulkanTexture2D(VulkanImage &image, VulkanDeviceMemory &deviceMemory, VkImageLayout layout, VulkanImageView &view, VulkanSampler &sampler) : m_image(&image), m_deviceMemory(&deviceMemory), m_view(&view), m_sampler(&sampler)
{
	m_descriptor.imageLayout = layout;
	m_descriptor.imageView = view.getHandle();
	m_descriptor.sampler = sampler.getHandle();
}

VulkanTexture2D::~VulkanTexture2D()
{

}

VkDescriptorImageInfo*
VulkanTexture2D::getImageInfo()
{
	return &m_descriptor;
}