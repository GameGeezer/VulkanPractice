#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class PipelineShaderStage
{
public:
	PipelineShaderStage();

	~PipelineShaderStage();

	void
	addShader(VkShaderModule shaderModule, char *entry, VkShaderStageFlagBits shaderStage);

	VkPipelineShaderStageCreateInfo*
	getCreateInfos();

	size_t
	size();

private:
	std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
};