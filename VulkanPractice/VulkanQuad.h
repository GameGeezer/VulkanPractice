#ifndef AMD_VULKAN_SAMPLE_QUAD_H_
#define AMD_VULKAN_SAMPLE_QUAD_H_

#include "VulkanSample.h"

class ShaderModule;
class PipelineLayout;
class GraphicsPipeline;
class VulkanBuffer;
class VulkanDevice;

namespace AMD
{
	

	class VulkanQuad : public VulkanSample
	{
	private:
		void CreatePipelineStateObject();
		void CreateMeshBuffers(VkCommandBuffer uploadCommandList);
		void RenderImpl(VkCommandBuffer commandList) override;
		void InitializeImpl(VkCommandBuffer uploadCommandList) override;
		void ShutdownImpl() override;

		VkDeviceMemory deviceMemory_ = VK_NULL_HANDLE;
		VulkanBuffer *vertexBuffer;
		VulkanBuffer *indexBuffer;

		ShaderModule *vertexShader_;
		ShaderModule *fragmentShader_;

		GraphicsPipeline *graphicsPipeline;
		VkPipeline pipeline_ = VK_NULL_HANDLE;
		PipelineLayout *pipelineLayout_;
	};
}

#endif