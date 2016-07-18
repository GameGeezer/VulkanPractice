#pragma once

#include <vulkan\vulkan.h>

class ImageView
{
public:
	ImageView(VkDevice device, VkImage image, VkFormat format);

	~ImageView();

	VkImageView
	getHandle();

private:
	VkDevice	m_device;
	VkImageView m_imageView;
};