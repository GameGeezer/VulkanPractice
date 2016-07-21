#include "GraphicsPipeline.h"

#include "PipelineLayout.h"

#include "VulkanShaderModule.h"

#include "PipelineVertexInputState.h"
#include "PipelineInputAssemblyState.h"

GraphicsPipeline::GraphicsPipeline(VkDevice device) : m_device(device)
{

}

GraphicsPipeline::~GraphicsPipeline()
{
	vkDestroyPipeline(m_device, m_pipeline, nullptr);
}

void
GraphicsPipeline::initialize(VkPipelineLayout layout, VkRenderPass renderPass, VkPipelineVertexInputStateCreateInfo vertexInput, VkPipelineInputAssemblyStateCreateInfo inputAssembly, VkPipelineViewportStateCreateInfo viewportState, VkPipelineColorBlendStateCreateInfo colorBlendState, VkPipelineRasterizationStateCreateInfo rasterizationState, VkPipelineDepthStencilStateCreateInfo depthStencilState, VkPipelineMultisampleStateCreateInfo multisampleState)
{
	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	graphicsPipelineCreateInfo.layout = layout;
	graphicsPipelineCreateInfo.renderPass = renderPass;
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInput;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssembly;
	graphicsPipelineCreateInfo.pViewportState = &viewportState;
	graphicsPipelineCreateInfo.pColorBlendState = &colorBlendState;
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationState;
	graphicsPipelineCreateInfo.pDepthStencilState = &depthStencilState;
	graphicsPipelineCreateInfo.pMultisampleState = &multisampleState;
	graphicsPipelineCreateInfo.pStages = m_shaderStages.data();
	graphicsPipelineCreateInfo.stageCount = static_cast<uint32_t> (m_shaderStages.size());
	
	vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo, nullptr, &m_pipeline);
}

void
GraphicsPipeline::addShaderStage(VkShaderModule &shadermodule, char *entry, VkShaderStageFlagBits stage)
{
	VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos = {};
	pipelineShaderStageCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineShaderStageCreateInfos.module = shadermodule;
	pipelineShaderStageCreateInfos.pName = entry;
	pipelineShaderStageCreateInfos.stage = stage;

	m_shaderStages.push_back(pipelineShaderStageCreateInfos);
}

VkPipeline
GraphicsPipeline::getHandle()
{
	return m_pipeline;
}