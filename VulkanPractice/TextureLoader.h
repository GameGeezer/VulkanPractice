#pragma once

#include <vulkan\vulkan.h>

#include <string>

class TextureLoader
{
public:
	void
	load(std::string fileName, VkFormat format);

};