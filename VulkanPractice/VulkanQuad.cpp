#include "VulkanQuad.h"

#include "Shaders.h"
#include "Utility.h"
#include "AMDWindow.h"
#include "VulkanShaderModule.h"
#include "PipelineLayout.h"
#include "PipelineColorBlendState.h"
#include "PipelineRasterizationState.h"
#include "PipelineDepthStencilState.h"
#include "PipelineMultisampleState.h"

#include "PipelineViewportState.h"
#include "PipelineInputAssemblyState.h"
#include "PipelineVertexInputState.h"
#include "GraphicsPipeline.h"

#include "VulkanBuffer.h"

#include "VulkanDevice.h"
#include "VulkanDeviceMemory.h"

#include <vector>

#include "VulkanRenderPass.h"

namespace AMD
{
	///////////////////////////////////////////////////////////////////////////////
	void VulkanQuad::ShutdownImpl()
	{
		vkDestroyPipeline(m_device->getDevice(), pipeline_, nullptr);
		delete pipelineLayout_;
		delete vertexBuffer;
		delete indexBuffer;
		vkFreeMemory(m_device->getDevice(), deviceMemory_, nullptr);

		delete vertexShader_;
		delete fragmentShader_;

		VulkanSample::ShutdownImpl();
	}

	///////////////////////////////////////////////////////////////////////////////
	void VulkanQuad::RenderImpl(VkCommandBuffer commandBuffer)
	{
		VulkanSample::RenderImpl(commandBuffer);

		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_);
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer->getHandle(), 0, VK_INDEX_TYPE_UINT32);
		VkBuffer vertBuffer = vertexBuffer->getHandle();
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, const_cast<const VkBuffer*>(&vertBuffer), offsets);
		vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
	}

	///////////////////////////////////////////////////////////////////////////////
	void VulkanQuad::InitializeImpl(VkCommandBuffer uploadCommandBuffer)
	{
		VulkanSample::InitializeImpl(uploadCommandBuffer);

		CreatePipelineStateObject();
		CreateMeshBuffers(uploadCommandBuffer);
	}

	namespace
	{
		///////////////////////////////////////////////////////////////////////////////
		VkPipelineLayout CreatePipelineLayout(VkDevice device)
		{
			VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
			pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

			VkPipelineLayout result;
			vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr,
				&result);

			return result;
		}

		///////////////////////////////////////////////////////////////////////////////
		GraphicsPipeline* CreatePipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout layout, VkShaderModule vertexShader, VkShaderModule fragmentShader, VkExtent2D viewportSize)
		{
			PipelineVertexInputState vertexInputState;
			vertexInputState.addVertexBindingDescription(0, VK_VERTEX_INPUT_RATE_VERTEX, sizeof(float) * 5);
			vertexInputState.addVertexInputAttributeDescription(0, VK_FORMAT_R32G32B32_SFLOAT, 0, 0);
			vertexInputState.addVertexInputAttributeDescription(0, VK_FORMAT_R32G32_SFLOAT, 1, sizeof(float) * 3);
			vertexInputState.initialize();

			PipelineInputAssemblyState assemblyState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

			PipelineViewportState viewportState;
			viewportState.addViewport(0, 0, viewportSize.width, viewportSize.height, 0, 1);
			viewportState.initialize();

			PipelineColorBlendState colorBlendState;
			colorBlendState.addAttachment(0xF, VK_FALSE);
			colorBlendState.initialize();

			PipelineRasterizationState rasterizationState(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, VK_FALSE, VK_FALSE, 1.0f);

			VkStencilOpState opState;
			opState.failOp = VK_STENCIL_OP_KEEP;
			opState.passOp = VK_STENCIL_OP_KEEP;
			opState.compareOp = VK_COMPARE_OP_ALWAYS;

			PipelineDepthStencilState depthStencilState(VK_FALSE, VK_FALSE, VK_COMPARE_OP_ALWAYS, VK_FALSE, opState, opState, VK_FALSE);

			PipelineMultisampleState multisampleState(VK_SAMPLE_COUNT_1_BIT);

			GraphicsPipeline *graphicsPipeline = new GraphicsPipeline(device);

			graphicsPipeline->addShaderStage(vertexShader, "main", VK_SHADER_STAGE_VERTEX_BIT);
			graphicsPipeline->addShaderStage(fragmentShader, "main", VK_SHADER_STAGE_FRAGMENT_BIT);
			graphicsPipeline->initialize(layout, renderPass, *vertexInputState.getCreateInfo(), *assemblyState.getCreateInfo(), *viewportState.getCreateInfo(), *colorBlendState.getCreateInfo(), *rasterizationState.getCreateInfo(), *depthStencilState.getCreateInfo(), *multisampleState.getCreateInfo());
	

			return graphicsPipeline;
		}
	}   // namespace

		///////////////////////////////////////////////////////////////////////////////
	void VulkanQuad::CreateMeshBuffers(VkCommandBuffer /*uploadCommandBuffer*/)
	{
		struct Vertex
		{
			float position[3];
			float uv[2];
		};

		static const Vertex vertices[4] =
		{
			// Upper Left
			{ { -1.0f,  1.0f, 0 },{ 0, 0 } },
			// Upper Right
			{ { 1.0f,  1.0f, 0 },{ 1, 0 } },
			// Bottom right
			{ { 1.0f, -1.0f, 0 },{ 1, 1 } },
			// Bottom left
			{ { -1.0f, -1.0f, 0 },{ 0, 1 } }
		};

		static const int indices[6] =
		{
			0, 1, 2, 2, 3, 0
		};

		vertexBuffer = new VulkanBuffer(m_device->getDevice(), sizeof(vertices), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		indexBuffer = new VulkanBuffer(m_device->getDevice(), sizeof(indices), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);


		VkDeviceSize bufferSize = vertexBuffer->getSize();
		// We want to place the index buffer behind the vertex buffer. Need to take
		// the alignment into account to find the next suitable location
		VkDeviceSize indexBufferOffset = RoundToNextMultiple(bufferSize, indexBuffer->getAlignment());

		bufferSize = indexBufferOffset + indexBuffer->getSize();
		deviceMemory_ = m_device->allocateMemory(static_cast<uint32_t>(bufferSize));

		VulkanDeviceMemory deviceMemory(m_device->getDevice(), deviceMemory_);
		deviceMemory.map(0, VK_WHOLE_SIZE);
		deviceMemory.copyInto(vertices, 0, sizeof(vertices));
		deviceMemory.copyInto(indices, static_cast<uint32_t>(indexBufferOffset), sizeof(indices));
		deviceMemory.unmap();

		vertexBuffer->bindToMemory(deviceMemory_, 0);
		indexBuffer->bindToMemory(deviceMemory_, static_cast<uint32_t>(indexBufferOffset));
	}

	///////////////////////////////////////////////////////////////////////////////
	void VulkanQuad::CreatePipelineStateObject()
	{
		vertexShader_ = new VulkanShaderModule(m_device->getDevice(), BasicVertexShader, sizeof(BasicVertexShader));
		fragmentShader_ = new VulkanShaderModule(m_device->getDevice(), BasicFragmentShader, sizeof(BasicFragmentShader));

		pipelineLayout_ = new PipelineLayout(m_device->getDevice());
		graphicsPipeline = CreatePipeline(m_device->getDevice(), window_->getRenderPass()->getHandle(), pipelineLayout_->getHandle(),
			vertexShader_->getHandle(), fragmentShader_->getHandle(),
			VkExtent2D{ static_cast<uint32_t> (640), static_cast<uint32_t> (480) }
		);

		pipeline_ = graphicsPipeline->getHandle();
	}
}   // namespace AMD