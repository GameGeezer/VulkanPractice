#pragma once

#include <vulkan\vulkan.h>

//	Images are not accessed directly in a shader, instead they
//	are wrapped in an ImageView and accessed through it
class VulkanImageView
{
public:
	//	device - Device used in creation and destruction of the image view
	//	image - The image being wrapped by the image view
	//	format - The formate used to interpret data in the image
	//	type - i.e. dimensionality of the image
	//	subresourceRange - Describes mipmap levels and array layers that are accessible
	VulkanImageView(VkDevice device, VkImage image, VkFormat format, VkImageViewType type, VkImageSubresourceRange &subresourceRange);
	
	~VulkanImageView();

	VkImageView
	getHandle();

private:
	VkDevice	m_device;
	VkImageView m_imageView;
};