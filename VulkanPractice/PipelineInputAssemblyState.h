#pragma once

#include <vulkan\vulkan.h>

class PipelineInputAssemblyeState
{
public:
	PipelineInputAssemblyeState(VkPrimitiveTopology topology);

	~PipelineInputAssemblyeState();

	VkPipelineInputAssemblyStateCreateInfo*
	getCreateInfo();

private:
	VkPipelineInputAssemblyStateCreateInfo m_pipelineInputAssemblyStateCreateInfo = {};
};