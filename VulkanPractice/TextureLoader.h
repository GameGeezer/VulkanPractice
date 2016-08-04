#pragma once

#include <vulkan\vulkan.h>

#include <string>

class VulkanDevice;
class VulkanCommandBuffer;
class VulkanTexture2D;

class TextureLoader
{
public:
	VulkanTexture2D*
	load(VulkanDevice &device, std::string fileName, VkFormat format, bool forceLinearTiling, VulkanCommandBuffer &commandBuffer, VkFence copyFence, uint64_t timeout);
};