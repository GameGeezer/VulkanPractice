#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class PipelineVertexInputState
{
public:
	PipelineVertexInputState();

	~PipelineVertexInputState();

	void
	initialize();

	void
	addVertexBindingDescription(uint32_t binding, VkVertexInputRate inputRate, uint32_t stride);

	void
	addVertexInputAttributeDescription(uint32_t binding, VkFormat format, uint32_t location, uint32_t offset);

	VkPipelineVertexInputStateCreateInfo*
	getCreateInfo();

private:
	VkPipelineVertexInputStateCreateInfo			m_pipelineVertexInputStateCreateInfo = {};
	std::vector<VkVertexInputBindingDescription>	m_vertexBindingDescriptions;
	std::vector<VkVertexInputAttributeDescription>	m_vertexInputAttributeDescriptions;
};