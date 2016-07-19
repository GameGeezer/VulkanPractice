#pragma once

#include <vulkan\vulkan.h>

class PipelineInputAssemblyState
{
public:
	PipelineInputAssemblyState(VkPrimitiveTopology topology);

	~PipelineInputAssemblyState();

	VkPipelineInputAssemblyStateCreateInfo*
	getCreateInfo();

private:
	VkPipelineInputAssemblyStateCreateInfo m_pipelineInputAssemblyStateCreateInfo = {};
};