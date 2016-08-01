#pragma once

#include <vulkan\vulkan.h>

class VulkanImageSubResourceRange
{
public:
	VulkanImageSubResourceRange(VkImageAspectFlagBits aspectMask, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount);

	~VulkanImageSubResourceRange();

	VkImageSubresourceRange*
	getRaw();

private:
	VkImageSubresourceRange m_imageSubResourceRange = {};
};