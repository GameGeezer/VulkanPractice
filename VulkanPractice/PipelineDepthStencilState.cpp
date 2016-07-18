#include "PipelineDepthStencilState.h"

PipelineDepthStencilState::PipelineDepthStencilState(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp, VkBool32 depthBoundsTestEnable, VkStencilOpState frontOpState, VkStencilOpState backOpState, VkBool32 stencilTestEnabled)
{
	m_pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	m_pipelineDepthStencilStateCreateInfo.depthTestEnable = depthTestEnable;
	m_pipelineDepthStencilStateCreateInfo.depthWriteEnable = depthWriteEnable;
	m_pipelineDepthStencilStateCreateInfo.depthCompareOp = depthCompareOp;
	m_pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = depthBoundsTestEnable;
	m_pipelineDepthStencilStateCreateInfo.back = backOpState;
	m_pipelineDepthStencilStateCreateInfo.front = frontOpState;
	m_pipelineDepthStencilStateCreateInfo.stencilTestEnable = stencilTestEnabled;	
}

PipelineDepthStencilState::~PipelineDepthStencilState()
{

}

VkPipelineDepthStencilStateCreateInfo*
PipelineDepthStencilState::getCreateInfo()
{
	return &m_pipelineDepthStencilStateCreateInfo;
}