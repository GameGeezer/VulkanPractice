#pragma once

#include <vulkan\vulkan.h>

class VulkanImage;
class VulkanDeviceMemory;
class VulkanImageView;
class VulkanSampler;

//	A texture wraps an image, the memory the image is bound to, a view of an image, and a sampler.
//	It wraps all the components necessary to access an image in a shader.
class VulkanTexture2D
{
public:
	VulkanTexture2D(VulkanImage &image, VulkanDeviceMemory &deviceMemory, VkImageLayout layout, VulkanImageView &view, VulkanSampler &sampler);

	~VulkanTexture2D();

	VkDescriptorImageInfo*
	getImageInfo();

private:
	VulkanImage				*	m_image;
	VulkanDeviceMemory		*	m_deviceMemory;
	VulkanImageView			*	m_view;
	VulkanSampler			*	m_sampler;
	VkDescriptorImageInfo		m_descriptor = {};

	uint32_t					m_width;
	uint32_t					m_height;
	uint32_t					m_mipLevels;
};