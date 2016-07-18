#pragma once

#include <vulkan\vulkan.h>

class PipelineDepthStencilState
{
public:
	PipelineDepthStencilState(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp, VkBool32 depthBoundsTestEnable, VkStencilOpState frontOpState, VkStencilOpState backOpState, VkBool32 stencilTestEnabled);

	~PipelineDepthStencilState();

	VkPipelineDepthStencilStateCreateInfo*
	getCreateInfo();

private:
	VkPipelineDepthStencilStateCreateInfo m_pipelineDepthStencilStateCreateInfo = {};
};