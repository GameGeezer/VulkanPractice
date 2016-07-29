#pragma once

#include <vulkan\vulkan.h>

class VulkanTexture
{

private:
	VkSampler sampler;
	VkImage image;
	VkImageLayout imageLayout;
	VkDeviceMemory deviceMemory;
	VkImageView view;
	VkDescriptorImageInfo descriptor;
	uint32_t width, height;
	uint32_t mipLevels;
};