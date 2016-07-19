#include "PipelineInputAssemblyState.h"

PipelineInputAssemblyState::PipelineInputAssemblyState(VkPrimitiveTopology topology)
{
	m_pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	m_pipelineInputAssemblyStateCreateInfo.topology = topology;	// How the primitives are assembled
}

PipelineInputAssemblyState::~PipelineInputAssemblyState()
{

}

VkPipelineInputAssemblyStateCreateInfo*
PipelineInputAssemblyState::getCreateInfo()
{
	return &m_pipelineInputAssemblyStateCreateInfo;
}