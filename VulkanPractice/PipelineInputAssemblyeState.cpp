#include "PipelineInputAssemblyState.h"

PipelineInputAssemblyeState::PipelineInputAssemblyeState(VkPrimitiveTopology topology)
{
	m_pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	m_pipelineInputAssemblyStateCreateInfo.topology = topology;
}

PipelineInputAssemblyeState::~PipelineInputAssemblyeState()
{

}

VkPipelineInputAssemblyStateCreateInfo*
PipelineInputAssemblyeState::getCreateInfo()
{
	return &m_pipelineInputAssemblyStateCreateInfo;
}