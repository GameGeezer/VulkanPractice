#pragma once

#include <vulkan\vulkan.h>

class PipelineMultisampleState
{
public:
	PipelineMultisampleState(VkSampleCountFlagBits rasterizationSamples);

	~PipelineMultisampleState();

	VkPipelineMultisampleStateCreateInfo*
	getCreateInfo();

private:
	VkPipelineMultisampleStateCreateInfo m_pipelineMultisampleStateCreateInfo = {};
};