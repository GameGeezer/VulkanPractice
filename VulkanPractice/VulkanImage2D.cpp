#include "VulkanImage2D.h"

VulkanImage2D::VulkanImage2D(VkDevice device,
	VkFormat format,
	VkImageLayout initialLayout,
	VkImageTiling tiling,
	VkImageUsageFlags usage,
	uint32_t mipMapLevels,
	uint32_t width,
	uint32_t height) : VulkanImage(device, VK_IMAGE_TYPE_2D, format, initialLayout, tiling, usage, mipMapLevels, width, height, 1)
{

}

VulkanImage2D::~VulkanImage2D()
{

}