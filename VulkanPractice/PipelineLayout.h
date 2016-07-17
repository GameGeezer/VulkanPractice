#pragma once

#include <vulkan\vulkan.h>

class PipelineLayout
{
public:
	PipelineLayout(VkDevice device);

	~PipelineLayout();

	VkPipelineLayout
	getHandle();

private:
	VkDevice			m_device;
	VkPipelineLayout	m_pipelineLayout;
};