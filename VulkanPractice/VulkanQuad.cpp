#include "VulkanQuad.h"

#include "Shaders.h"
#include "Utility.h"
#include "AMDWindow.h"
#include "ShaderModule.h"
#include "PipelineLayout.h"
#include "PipelineColorBlendState.h"
#include "PipelineRasterizationState.h"
#include "PipelineDepthStencilState.h"
#include "PipelineMultisampleState.h"
#include "PipelineShaderStage.h"
#include "PipelineViewportState.h"
#include "PipelineInputAssemblyState.h"
#include "PipelineVertexInputState.h"

#include <vector>

#include "RenderPass.h"

namespace AMD
{
	///////////////////////////////////////////////////////////////////////////////
	void VulkanQuad::ShutdownImpl()
	{
		vkDestroyPipeline(device_, pipeline_, nullptr);
		delete pipelineLayout_;

		vkDestroyBuffer(device_, vertexBuffer_, nullptr);
		vkDestroyBuffer(device_, indexBuffer_, nullptr);
		vkFreeMemory(device_, deviceMemory_, nullptr);

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
		vkCmdBindIndexBuffer(commandBuffer, indexBuffer_, 0, VK_INDEX_TYPE_UINT32);
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer_, offsets);
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
		struct MemoryTypeInfo
		{
			bool deviceLocal = false;
			bool hostVisible = false;
			bool hostCoherent = false;
			bool hostCached = false;
			bool lazilyAllocated = false;

			struct Heap
			{
				uint64_t size = 0;
				bool deviceLocal = false;
			};

			Heap heap;
			int index;
		};

		///////////////////////////////////////////////////////////////////////////////
		std::vector<MemoryTypeInfo> EnumerateHeaps(VkPhysicalDevice device)
		{
			VkPhysicalDeviceMemoryProperties memoryProperties = {};
			vkGetPhysicalDeviceMemoryProperties(device, &memoryProperties);

			std::vector<MemoryTypeInfo::Heap> heaps;

			for (uint32_t i = 0; i < memoryProperties.memoryHeapCount; ++i)
			{
				MemoryTypeInfo::Heap info;
				info.size = memoryProperties.memoryHeaps[i].size;
				info.deviceLocal = (memoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0;

				heaps.push_back(info);
			}

			std::vector<MemoryTypeInfo> result;

			for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
			{
				MemoryTypeInfo typeInfo;

				typeInfo.deviceLocal = (memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0;
				typeInfo.hostVisible = (memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0;
				typeInfo.hostCoherent = (memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0;
				typeInfo.hostCached = (memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) != 0;
				typeInfo.lazilyAllocated = (memoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) != 0;

				typeInfo.heap = heaps[memoryProperties.memoryTypes[i].heapIndex];

				typeInfo.index = static_cast<int> (i);

				result.push_back(typeInfo);
			}

			return result;
		}

		///////////////////////////////////////////////////////////////////////////////
		VkDeviceMemory AllocateMemory(const std::vector<MemoryTypeInfo>& memoryInfos, VkDevice device, const int size)
		{
			// We take the first HOST_VISIBLE memory
			for (auto& memoryInfo : memoryInfos)
			{
				if (memoryInfo.hostVisible)
				{
					VkMemoryAllocateInfo memoryAllocateInfo = {};
					memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
					memoryAllocateInfo.memoryTypeIndex = memoryInfo.index;
					memoryAllocateInfo.allocationSize = size;

					VkDeviceMemory deviceMemory;
					vkAllocateMemory(device, &memoryAllocateInfo, nullptr,
						&deviceMemory);
					return deviceMemory;
				}
			}

			return VK_NULL_HANDLE;
		}

		///////////////////////////////////////////////////////////////////////////////
		VkBuffer AllocateBuffer(VkDevice device, const int size, const VkBufferUsageFlagBits bits)
		{
			VkBufferCreateInfo bufferCreateInfo = {};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.size = static_cast<uint32_t> (size);
			bufferCreateInfo.usage = bits;

			VkBuffer result;
			vkCreateBuffer(device, &bufferCreateInfo, nullptr, &result);
			return result;
		}

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
		VkPipeline CreatePipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout layout, VkShaderModule vertexShader, VkShaderModule fragmentShader, VkExtent2D viewportSize)
		{
			PipelineVertexInputState vertexInputState;
			vertexInputState.addVertexBindingDescription(0, VK_VERTEX_INPUT_RATE_VERTEX, sizeof(float) * 5);
			vertexInputState.addVertexInputAttributeDescription(0, VK_FORMAT_R32G32B32_SFLOAT, 0, 0);
			vertexInputState.addVertexInputAttributeDescription(0, VK_FORMAT_R32G32_SFLOAT, 1, sizeof(float) * 3);
			vertexInputState.initialize();

			PipelineInputAssemblyeState assemblyState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);

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

			PipelineShaderStage shaderStage;
			shaderStage.addShader(vertexShader, "main", VK_SHADER_STAGE_VERTEX_BIT);
			shaderStage.addShader(fragmentShader, "main", VK_SHADER_STAGE_FRAGMENT_BIT);

			VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo = {};
			graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

			graphicsPipelineCreateInfo.layout = layout;
			graphicsPipelineCreateInfo.pVertexInputState = vertexInputState.getCreateInfo();
			graphicsPipelineCreateInfo.pInputAssemblyState = assemblyState.getCreateInfo();
			graphicsPipelineCreateInfo.renderPass = renderPass;
			graphicsPipelineCreateInfo.pViewportState = viewportState.getCreateInfo();
			graphicsPipelineCreateInfo.pColorBlendState = colorBlendState.getCreateInfo();
			graphicsPipelineCreateInfo.pRasterizationState = rasterizationState.getCreateInfo();
			graphicsPipelineCreateInfo.pDepthStencilState = depthStencilState.getCreateInfo();
			graphicsPipelineCreateInfo.pMultisampleState = multisampleState.getCreateInfo();
			graphicsPipelineCreateInfo.pStages = shaderStage.getCreateInfos();
			graphicsPipelineCreateInfo.stageCount = static_cast<uint32_t> (shaderStage.size());

			VkPipeline pipeline;
			vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &graphicsPipelineCreateInfo,
				nullptr, &pipeline);

			return pipeline;
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

		auto memoryHeaps = EnumerateHeaps(physicalDevice_);

		indexBuffer_ = AllocateBuffer(device_, sizeof(indices),
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
		vertexBuffer_ = AllocateBuffer(device_, sizeof(vertices),
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);

		VkMemoryRequirements vertexBufferMemoryRequirements = {};
		vkGetBufferMemoryRequirements(device_, vertexBuffer_,
			&vertexBufferMemoryRequirements);
		VkMemoryRequirements indexBufferMemoryRequirements = {};
		vkGetBufferMemoryRequirements(device_, indexBuffer_,
			&indexBufferMemoryRequirements);

		VkDeviceSize bufferSize = vertexBufferMemoryRequirements.size;
		// We want to place the index buffer behind the vertex buffer. Need to take
		// the alignment into account to find the next suitable location
		VkDeviceSize indexBufferOffset = RoundToNextMultiple(bufferSize,
			indexBufferMemoryRequirements.alignment);

		bufferSize = indexBufferOffset + indexBufferMemoryRequirements.size;
		deviceMemory_ = AllocateMemory(memoryHeaps, device_,
			static_cast<int>(bufferSize));

		vkBindBufferMemory(device_, vertexBuffer_, deviceMemory_, 0);
		vkBindBufferMemory(device_, indexBuffer_, deviceMemory_,
			indexBufferOffset);

		void* mapping = nullptr;
		vkMapMemory(device_, deviceMemory_, 0, VK_WHOLE_SIZE,
			0, &mapping);
		::memcpy(mapping, vertices, sizeof(vertices));

		::memcpy(static_cast<uint8_t*> (mapping) + indexBufferOffset,
			indices, sizeof(indices));
		vkUnmapMemory(device_, deviceMemory_);
	}

	///////////////////////////////////////////////////////////////////////////////
	void VulkanQuad::CreatePipelineStateObject()
	{
		vertexShader_ = new ShaderModule(device_, BasicVertexShader, sizeof(BasicVertexShader));
		fragmentShader_ = new ShaderModule(device_, BasicFragmentShader, sizeof(BasicFragmentShader));

		pipelineLayout_ = new PipelineLayout(device_);
		pipeline_ = CreatePipeline(device_, renderPass->getHandle(), pipelineLayout_->getHandle(),
			vertexShader_->getHandle(), fragmentShader_->getHandle(),
			VkExtent2D{ static_cast<uint32_t> (window_->GetWidth()), static_cast<uint32_t> (window_->GetHeight()) }
		);
	}
}   // namespace AMD