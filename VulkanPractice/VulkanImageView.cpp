#include "VulkanImageView.h"

VulkanImageView::VulkanImageView(VkDevice device, VkImage image, VkFormat format, VkImageViewType type, VkImageSubresourceRange &subresourceRange) : m_device(device)
{
	VkImageViewCreateInfo imageViewCreateInfo = {};
	imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	imageViewCreateInfo.image = image;
	imageViewCreateInfo.viewType = type;
	imageViewCreateInfo.format = format;
	imageViewCreateInfo.subresourceRange.levelCount = 1;
	imageViewCreateInfo.subresourceRange.layerCount = 1;
	imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

	vkCreateImageView(m_device, &imageViewCreateInfo, nullptr, &m_imageView);
}

VulkanImageView::~VulkanImageView()
{
	vkDestroyImageView(m_device, m_imageView, nullptr);
}

VkImageView
VulkanImageView::getHandle()
{
	return m_imageView;
}