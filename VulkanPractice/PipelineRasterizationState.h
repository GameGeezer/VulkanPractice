#pragma once

#include <vulkan\vulkan.h>

class PipelineRasterizationState
{
public:
	PipelineRasterizationState(VkPolygonMode polygonMode, VkCullModeFlags cullModeFlags, VkFrontFace frontFace, VkBool32 depthClampEnabled, VkBool32 rasterizerDiscardEnabled, VkBool32 depthBiasEnable, float lineWidth);

	~PipelineRasterizationState();

	VkPipelineRasterizationStateCreateInfo*
	getCreateInfo();

private:
	VkPipelineRasterizationStateCreateInfo m_pipelineRasterizationStateCreateInfo = {};
};