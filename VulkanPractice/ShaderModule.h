#pragma once

#include <vulkan\vulkan.h>

class ShaderModule
{
public:
	ShaderModule(VkDevice device, const void *contents, uint32_t codeSizeBytes);

	~ShaderModule();

	VkShaderModule
	getHandle();

private:
	VkDevice		m_device;
	VkShaderModule	m_shaderModule;
};