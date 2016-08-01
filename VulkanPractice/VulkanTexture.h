#pragma once

#include <vulkan\vulkan.h>

class VulkanTexture
{
public:
	VulkanTexture(VkImage image);

	~VulkanTexture();

private:
	VkImage image;
	VkDeviceMemory deviceMemory;
	VkDescriptorImageInfo descriptor;
	uint32_t width, height;
	uint32_t mipLevels;
};