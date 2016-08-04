#pragma once

#include <vulkan\vulkan.h>

class VulkanImageSubResourceRange
{
public:
	//	aspectMask		-	Which image aspects are included
	//	baseMipLevel	-	The first accessible mipMap level (must not be 0)
	//	levelCount		-	The number of accessible mipMap levels (must be less than the remaining mipMap levels)
	//						~VK_REMAINING_MIP_LEVELS - All remaining mipMap levels
	//	baseArrayLayer	-	The first accessible array layer
	//	layerCount		-	The number of accessible array layers
	//						~VK_REMAINING_ARRAY_LAYERS - All remaining Layers
	VulkanImageSubResourceRange(VkImageAspectFlags aspectMask, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);

	~VulkanImageSubResourceRange();

	VkImageSubresourceRange*
	getRaw();

private:
	VkImageSubresourceRange m_imageSubResourceRange = {};
};