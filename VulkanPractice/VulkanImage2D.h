#pragma once

#include "VulkanImage.h"

//	This is a convenience class that fills in VulkanImage parameters that are
//	consistant between 2D images.
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