#include "BasicVertexPipeline.h"

#include "VulkanDescriptorSetLayout.h"
#include "PipelineLayout.h"

#include "PipelineVertexInputState.h"
#include "PipelineInputAssemblyState.h"
#include "PipelineViewportState.h"
#include "PipelineColorBlendState.h"
#include "PipelineRasterizationState.h"
#include "PipelineDepthStencilState.h"
#include "PipelineMultisampleState.h"
#include "GraphicsPipeline.h"

#include "VertexPos3Col3Tex2.h"

BasicVertexPipeline::BasicVertexPipeline(VkDevice device, VkExtent2D viewportSize, VkRenderPass renderPass) : m_device(device), m_viewportExtent(viewportSize), m_renderPass(renderPass)
{
	
}

BasicVertexPipeline::~BasicVertexPipeline()
{

}

void
BasicVertexPipeline::build(VkShaderModule vertexShader, VkShaderModule fragmentShader)
{
	VulkanDescriptorSetLayout uniformLayout(m_device);
	uniformLayout.addBinding(0, 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT);
	uniformLayout.addBinding(1, 1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
	uniformLayout.initialize();

	if (m_layout != nullptr)
	{
		delete m_layout;
	}

	m_layout = new PipelineLayout(m_device);
	m_layout->addDescriptorSetLayout(uniformLayout.getHandle());
	m_layout->initialize();

	PipelineVertexInputState vertexInputState;
	VertexPos3Col3Tex2::addToPipeline(0, vertexInputState);
	vertexInputState.initialize();

	PipelineInputAssemblyState assemblyState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

	PipelineViewportState viewportState;
	viewportState.addViewport(0, 0, m_viewportExtent.width, m_viewportExtent.height, 0, 1);
	viewportState.initialize();

	PipelineColorBlendState colorBlendState;
	colorBlendState.addAttachment(0xF, VK_FALSE);
	colorBlendState.initialize();

	PipelineRasterizationState rasterizationState(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, VK_FALSE, VK_FALSE, 1.0f);

	VkStencilOpState opState;
	opState.failOp = VK_STENCIL_OP_KEEP;
	opState.passOp = VK_STENCIL_OP_KEEP;
	opState.compareOp = VK_COMPARE_OP_ALWAYS;

	PipelineDepthStencilState depthStencilState(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS, VK_FALSE, {}, {}, VK_FALSE);

	PipelineMultisampleState multisampleState(VK_SAMPLE_COUNT_1_BIT);

	GraphicsPipeline *graphicsPipeline = new GraphicsPipeline(m_device);

	graphicsPipeline->addShaderStage(vertexShader, "main", VK_SHADER_STAGE_VERTEX_BIT);
	graphicsPipeline->addShaderStage(fragmentShader, "main", VK_SHADER_STAGE_FRAGMENT_BIT);
	graphicsPipeline->initialize(m_layout->getHandle(), m_renderPass, *vertexInputState.getCreateInfo(), *assemblyState.getCreateInfo(), *viewportState.getCreateInfo(), *colorBlendState.getCreateInfo(), *rasterizationState.getCreateInfo(), *depthStencilState.getCreateInfo(), *multisampleState.getCreateInfo());
}