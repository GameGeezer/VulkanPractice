#include "PipelineColorBlendState.h"

PipelineColorBlendState::PipelineColorBlendState()
{

}

PipelineColorBlendState::~PipelineColorBlendState()
{

}

void
PipelineColorBlendState::initialize()
{
	m_pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;

	m_pipelineColorBlendStateCreateInfo.attachmentCount = (uint32_t) m_colorBlendAttachments.size();
	m_pipelineColorBlendStateCreateInfo.pAttachments = m_colorBlendAttachments.data();
}

void
PipelineColorBlendState::addAttachment(uint32_t colorWriteMask, VkBool32 blend)
{
	VkPipelineColorBlendAttachmentState pipelineColorBlendAttachmentState = {};
	pipelineColorBlendAttachmentState.colorWriteMask = 0xF;
	pipelineColorBlendAttachmentState.blendEnable = VK_FALSE;

	m_colorBlendAttachments.push_back(pipelineColorBlendAttachmentState);
}

VkPipelineColorBlendStateCreateInfo*
PipelineColorBlendState::getCreateInfo()
{
	return &m_pipelineColorBlendStateCreateInfo;
}