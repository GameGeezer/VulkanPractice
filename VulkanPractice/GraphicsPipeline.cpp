#include "GraphicsPipeline.h"

#include "PipelineLayout.h"

#include "ShaderModule.h"

GraphicsPipeline::GraphicsPipeline(VkDevice device, PipelineLayout& layout) 
{

}

GraphicsPipeline::~GraphicsPipeline()
{

}

void
GraphicsPipeline::initialize()
{
	/*
	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

	graphicsPipelineCreateInfo.layout = layout;
	graphicsPipelineCreateInfo.pVertexInputState = &pipelineVertexInputStateCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &pipelineInputAssemblyStateCreateInfo;
	graphicsPipelineCreateInfo.renderPass = renderPass;
	graphicsPipelineCreateInfo.pViewportState = &pipelineViewportStateCreateInfo;
	graphicsPipelineCreateInfo.pColorBlendState = &pipelineColorBlendStateCreateInfo;
	graphicsPipelineCreateInfo.pRasterizationState = &pipelineRasterizationStateCreateInfo;
	graphicsPipelineCreateInfo.pDepthStencilState = &pipelineDepthStencilStateCreateInfo;
	graphicsPipelineCreateInfo.pMultisampleState = &pipelineMultisampleStateCreateInfo;
	graphicsPipelineCreateInfo.pStages = m_shaderStages.data();
	graphicsPipelineCreateInfo.stageCount = m_shaderStages.size();
	*/
}

void
GraphicsPipeline::addShaderStage(ShaderModule &shadermodule, char *entry, VkShaderStageFlagBits stage)
{
	VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfos = {};
	pipelineShaderStageCreateInfos.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineShaderStageCreateInfos.module = shadermodule.getHandle();
	pipelineShaderStageCreateInfos.pName = entry;
	pipelineShaderStageCreateInfos.stage = stage;

	m_shaderStages.push_back(pipelineShaderStageCreateInfos);
}