#include "PipelineViewportState.h"

PipelineViewportState::PipelineViewportState()
{

}

PipelineViewportState::~PipelineViewportState()
{

}

void
PipelineViewportState::initialize()
{
	m_pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	m_pipelineViewportStateCreateInfo.viewportCount = static_cast<uint32_t> (m_viewports.size());
	m_pipelineViewportStateCreateInfo.pViewports = m_viewports.data();
	m_pipelineViewportStateCreateInfo.scissorCount = static_cast<uint32_t> (m_scissors.size());
	m_pipelineViewportStateCreateInfo.pScissors = m_scissors.data();
}

void
PipelineViewportState::addViewport(float x, float y, uint32_t width, uint32_t height, float minDepth, float maxDepth)
{
	VkViewport viewport;
	viewport.x = x;
	viewport.y = y;
	viewport.width = static_cast<float> (width);
	viewport.height = static_cast<float> (height);
	viewport.minDepth = minDepth;
	viewport.maxDepth = maxDepth;

	m_viewports.push_back(viewport);

	VkExtent2D viewportSize;
	viewportSize.width = width;
	viewportSize.height = height;

	VkRect2D rect;
	rect.extent = viewportSize;
	rect.offset.x = 0;
	rect.offset.y = 0;

	m_scissors.push_back(rect);
}

VkPipelineViewportStateCreateInfo*
PipelineViewportState::getCreateInfo()
{
	return &m_pipelineViewportStateCreateInfo;
}