#include "VulkanShaderModule.h"

VulkanShaderModule::VulkanShaderModule(VkDevice device, const void *contents, uint32_t codeSizeBytes) : m_device(device)
{
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.pCode = static_cast<const uint32_t*>(contents);
	shaderModuleCreateInfo.codeSize = codeSizeBytes;

	vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &m_shaderModule);
}

VulkanShaderModule::~VulkanShaderModule()
{
	vkDestroyShaderModule(m_device, m_shaderModule, nullptr);
}

VkShaderModule
VulkanShaderModule::getHandle()
{
	return m_shaderModule;
}