#pragma once

#include <vulkan\vulkan.h>

class VulkanImageMemoryBarrier
{
public:
	VulkanImageMemoryBarrier(VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkImageSubresourceRange subresourceRange);

	~VulkanImageMemoryBarrier();

	VkImageMemoryBarrier*
	getRaw();

private:
	VkImageMemoryBarrier m_imageMemoryBarrier = {};
};
