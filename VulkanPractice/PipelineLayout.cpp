#include "PipelineLayout.h"

PipelineLayout::PipelineLayout(VkDevice device) : m_device(device)
{

}

void
PipelineLayout::initialize()
{
	if (m_initialized)
	{
		throw "PipelineLayout has already been initialized";
	}
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(m_descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = m_descriptorSetLayouts.data();

	vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayout);

	m_initialized = true;
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

void
PipelineLayout::addDescriptorSetLayout(VkDescriptorSetLayout layout)
{
	m_descriptorSetLayouts.push_back(layout);
}