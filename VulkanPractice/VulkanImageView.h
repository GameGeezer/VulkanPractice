#pragma once

#include <vulkan\vulkan.h>

class VulkanImageView
{
public:
	VulkanImageView(VkDevice device, VkImage image, VkFormat format);

	~VulkanImageView();

	VkImageView
	getHandle();

private:
	VkDevice	m_device;
	VkImageView m_imageView;
};