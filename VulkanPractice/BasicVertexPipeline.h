#pragma once

#include <vulkan\vulkan.h>

class PipelineLayout;
class GraphicsPipeline;

class BasicVertexPipeline
{
public:
	BasicVertexPipeline(VkDevice device, VkExtent2D viewportSize, VkRenderPass renderPass);

	~BasicVertexPipeline();
	
	void
	build(VkShaderModule vertexShader, VkShaderModule fragmentShader);

private:
	VkDevice				m_device;
	VkExtent2D				m_viewportExtent;
	VkRenderPass			m_renderPass;

	PipelineLayout		*	m_layout = nullptr;
	GraphicsPipeline	*	m_graphicsPipeline = nullptr;
};