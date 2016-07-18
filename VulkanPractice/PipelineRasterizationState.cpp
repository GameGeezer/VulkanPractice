
#include "PipelineRasterizationState.h"

PipelineRasterizationState::PipelineRasterizationState(VkPolygonMode polygonMode, VkCullModeFlags cullModeFlags, VkFrontFace frontFace, VkBool32 depthClampEnabled, VkBool32 rasterizerDiscardEnabled, VkBool32 depthBiasEnable, float lineWidth)
{
	VkPipelineRasterizationStateCreateInfo pipelineRasterizationStateCreateInfo = {};
	pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	pipelineRasterizationStateCreateInfo.polygonMode = polygonMode;
	pipelineRasterizationStateCreateInfo.cullMode = cullModeFlags;
	pipelineRasterizationStateCreateInfo.frontFace = frontFace;
	pipelineRasterizationStateCreateInfo.depthClampEnable = depthClampEnabled;
	pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = rasterizerDiscardEnabled;
	pipelineRasterizationStateCreateInfo.depthBiasEnable = depthBiasEnable;
	pipelineRasterizationStateCreateInfo.lineWidth = lineWidth;
}

PipelineRasterizationState::~PipelineRasterizationState()
{

}

VkPipelineRasterizationStateCreateInfo*
PipelineRasterizationState::getCreateInfo()
{
	return &m_pipelineRasterizationStateCreateInfo;
}