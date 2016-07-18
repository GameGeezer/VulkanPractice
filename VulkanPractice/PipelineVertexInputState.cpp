#include "PipelineVertexInputState.h"

PipelineVertexInputState::PipelineVertexInputState()
{

}

PipelineVertexInputState::~PipelineVertexInputState()
{

}

void
PipelineVertexInputState::initialize()
{
	m_pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	m_pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t> (m_vertexInputAttributeDescriptions.size());
	m_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = m_vertexInputAttributeDescriptions.data();
	m_pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(m_vertexBindingDescriptions.size());
	m_pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = m_vertexBindingDescriptions.data();
}

void
PipelineVertexInputState::addVertexBindingDescription(uint32_t binding, VkVertexInputRate inputRate, uint32_t stride)
{
	VkVertexInputBindingDescription vertexInputBindingDescription;
	vertexInputBindingDescription.binding = binding;
	vertexInputBindingDescription.inputRate = inputRate;
	vertexInputBindingDescription.stride = stride;

	m_vertexBindingDescriptions.push_back(vertexInputBindingDescription);
}

void
PipelineVertexInputState::addVertexInputAttributeDescription(uint32_t binding, VkFormat format, uint32_t location, uint32_t offset)
{
	VkVertexInputAttributeDescription vertexInputAttributeDescription = {};
	vertexInputAttributeDescription.binding = binding;
	vertexInputAttributeDescription.format = format;
	vertexInputAttributeDescription.location = location;
	vertexInputAttributeDescription.offset = offset;

	m_vertexInputAttributeDescriptions.push_back(vertexInputAttributeDescription);
}

VkPipelineVertexInputStateCreateInfo*
PipelineVertexInputState::getCreateInfo()
{
	return &m_pipelineVertexInputStateCreateInfo;
}