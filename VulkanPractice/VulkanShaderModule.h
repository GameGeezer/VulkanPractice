#pragma once

#include <vulkan\vulkan.h>

class VulkanShaderModule
{
public:
	VulkanShaderModule(VkDevice device, const void *contents, uint32_t codeSizeBytes);

	~VulkanShaderModule();

	VkShaderModule
	getHandle();

private:
	VkDevice		m_device;
	VkShaderModule	m_shaderModule;
};