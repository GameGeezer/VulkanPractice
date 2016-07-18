#include "PipelineMultisampleState.h"

PipelineMultisampleState::PipelineMultisampleState(VkSampleCountFlagBits rasterizationSamples)
{
	m_pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	m_pipelineMultisampleStateCreateInfo.rasterizationSamples = rasterizationSamples;
}

PipelineMultisampleState::~PipelineMultisampleState()
{

}

VkPipelineMultisampleStateCreateInfo*
PipelineMultisampleState::getCreateInfo()
{
	return &m_pipelineMultisampleStateCreateInfo;
}