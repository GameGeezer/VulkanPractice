#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class PipelineLayout;
class ShaderModule;

class GraphicsPipeline
{
public:
	GraphicsPipeline(VkDevice device);

	~GraphicsPipeline();

	void
	initialize(VkPipelineLayout layout, VkRenderPass renderPass, VkPipelineVertexInputStateCreateInfo vertexInput, VkPipelineInputAssemblyStateCreateInfo inputAssembly, VkPipelineViewportStateCreateInfo viewportState, VkPipelineColorBlendStateCreateInfo colorBlendState, VkPipelineRasterizationStateCreateInfo rasterizationState, VkPipelineDepthStencilStateCreateInfo depthStencilState, VkPipelineMultisampleStateCreateInfo multisampleState);

	void
	addShaderStage(VkShaderModule &shadermodule, char *entry, VkShaderStageFlagBits stage);

	VkPipeline
	getHandle();

private:
	VkDevice	m_device;
	VkPipeline	m_pipeline;

	std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;
};