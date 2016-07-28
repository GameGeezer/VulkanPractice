#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class PipelineLayout
{
public:
	PipelineLayout(VkDevice device);

	~PipelineLayout();

	void
	initialize();

	VkPipelineLayout
	getHandle();

	void
	addDescriptorSetLayout(VkDescriptorSetLayout layout);

private:
	VkDevice							m_device;
	VkPipelineLayout					m_pipelineLayout;

	std::vector<VkDescriptorSetLayout>	m_descriptorSetLayouts;
	bool								m_initialized = false;
};