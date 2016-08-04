#pragma once

#include "VulkanImage.h"

class VulkanImage2D : public VulkanImage
{
public:
	VulkanImage2D(VkDevice device,
		VkFormat format,
		VkImageLayout initialLayout,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		uint32_t mipMapLevels,
		uint32_t width,
		uint32_t height);

	~VulkanImage2D();

};