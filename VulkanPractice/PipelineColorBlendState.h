#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class PipelineColorBlendState
{
public:
	PipelineColorBlendState();

	~PipelineColorBlendState();

	void
	initialize();

	void
	addAttachment(uint32_t colorWriteMask, VkBool32 blend);

	VkPipelineColorBlendStateCreateInfo*
	getCreateInfo();

private:
	VkPipelineColorBlendStateCreateInfo					m_pipelineColorBlendStateCreateInfo = {};
	std::vector<VkPipelineColorBlendAttachmentState>	m_colorBlendAttachments;
};
