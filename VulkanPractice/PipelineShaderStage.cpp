#include "PipelineShaderStage.h"

#include "ShaderModule.h"

PipelineShaderStage::PipelineShaderStage()
{

}

PipelineShaderStage::~PipelineShaderStage()
{

}

void
PipelineShaderStage::addShader(VkShaderModule shaderModule, char *entry, VkShaderStageFlagBits shaderStage)
{
	VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
	pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineShaderStageCreateInfo.module = shaderModule;
	pipelineShaderStageCreateInfo.pName = entry;
	pipelineShaderStageCreateInfo.stage = shaderStage;

	m_shaderStages.push_back(pipelineShaderStageCreateInfo);
}

VkPipelineShaderStageCreateInfo*
PipelineShaderStage::getCreateInfos()
{
	return m_shaderStages.data();
}

size_t
PipelineShaderStage::size()
{
	return m_shaderStages.size();
}