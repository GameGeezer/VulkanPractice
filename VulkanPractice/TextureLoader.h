#pragma once

#include <vulkan\vulkan.h>

#include <string>

class VulkanDevice;

class TextureLoader
{
public:
	void
	load(VulkanDevice &device, std::string fileName, VkFormat format);
};