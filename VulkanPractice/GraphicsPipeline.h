#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class PipelineLayout;
class ShaderModule;

class GraphicsPipeline
{
public:
	GraphicsPipeline(VkDevice device, PipelineLayout& layout);

	~GraphicsPipeline();

	void
	initialize();

	void
	addShaderStage(ShaderModule &shadermodule, char *entry, VkShaderStageFlagBits stage);

private:
	VkDevice	m_device;
	VkPipeline	m_pipeline;

	std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
};