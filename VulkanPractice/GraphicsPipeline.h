#pragma once

#include <vulkan\vulkan.h>

class PipelineLayout;

class GraphicsPipeline
{
public:
	GraphicsPipeline(VkDevice device, PipelineLayout& layout);

	~GraphicsPipeline();

private:
	VkDevice	m_device;
	VkPipeline	m_pipeline;
};