#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class PipelineViewportState
{
public:
	PipelineViewportState();

	~PipelineViewportState();

	void
	initialize();

	void
	addViewport(float x, float y, uint32_t width, uint32_t height, float minDepth, float maxDepth);

	VkPipelineViewportStateCreateInfo*
	getCreateInfo();

private:
	VkPipelineViewportStateCreateInfo	m_pipelineViewportStateCreateInfo = {};
	std::vector<VkViewport>				m_viewports;
	std::vector<VkRect2D>				m_scissors;
};