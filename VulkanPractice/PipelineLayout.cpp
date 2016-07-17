#include "PipelineLayout.h"

PipelineLayout::PipelineLayout(VkDevice device) : m_device(device)
{
	VkPipelineLayoutCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	vkCreatePipelineLayout(device, &createInfo, nullptr, &m_pipelineLayout);
}

PipelineLayout::~PipelineLayout()
{
	vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
}

VkPipelineLayout
PipelineLayout::getHandle()
{
	return m_pipelineLayout;
}