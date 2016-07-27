#include "TestScreen.h"

#include "Window.h"
#include "PipelineVertexInputState.h"
#include "PipelineInputAssemblyState.h"
#include "PipelineViewportState.h"
#include "PipelineColorBlendState.h"
#include "PipelineRasterizationState.h"
#include "PipelineDepthStencilState.h"
#include "PipelineMultisampleState.h"
#include "GraphicsPipeline.h"
#include "VulkanDeviceMemory.h"
#include "VulkanBuffer.h"
#include "VulkanShaderModule.h"
#include "VulkanRenderPass.h"
#include "VulkanFenceGroup.h"
#include "VulkanCommandBufferGroup.h"
#include "VulkanCommandBuffer.h"
#include "VulkanQueueSubmission.h"

#include "Application.h"
#include "VulkanDevice.h"
#include "PipelineLayout.h"

#include "Shaders.h"


#include "Utility.h"

#include <glm\mat4x4.hpp>

struct {
	glm::mat4 projectionMatrix;
	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
} uboVS;

void
TestScreen::onCreate()
{
	VulkanCommandBuffer *setupCommandBuffer = getApplication()->getCommandBuffers()->getCommandBufferAtIndex(0);
	VulkanFenceGroup *fences = getApplication()->getFences();

	fences->resetFences(0, 1);
	{
		setupCommandBuffer->begin(VkCommandBufferUsageFlagBits::VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		createPipelineStateObject();
		createMeshBuffers(setupCommandBuffer->getHandle());
		setupCommandBuffer->end();

		// What to submit to the queue
		VkCommandBuffer buf = setupCommandBuffer->getHandle();
		VulkanQueueSubmission submission;
		submission.addCommandBuffer(buf);
		submission.inititialize(0);
		// Submit the command buffer to the queue
		getApplication()->getDevice()->submitToQueue(1, submission.getSubmitInfo(), fences->getFenceAtIndex(0));
	}
}

void
TestScreen::onPause()
{

}

void
TestScreen::onLeave()
{

}

void
TestScreen::onResume()
{

}

void
TestScreen::onUpdate(uint32_t delta)
{

}

void
TestScreen::onRender(VulkanCommandBuffer *commandBufferWrapper)
{
	VkCommandBuffer commandBuffer = commandBufferWrapper->getHandle();
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline->getHandle());
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer->getHandle(), 0, VK_INDEX_TYPE_UINT32);
	VkBuffer vertBuffer = m_vertexBuffer->getHandle();
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, const_cast<const VkBuffer*>(&vertBuffer), offsets);
	vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
}

void
TestScreen::onResize(uint32_t width, uint32_t height)
{

}

void
TestScreen::onDestroy()
{

}

GraphicsPipeline* TestScreen::
createPipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout layout, VkShaderModule vertexShader, VkShaderModule fragmentShader, VkExtent2D viewportSize)
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

void
TestScreen::createMeshBuffers(VkCommandBuffer uploadCommandBuffer)
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

	VulkanDevice *device = getApplication()->getDevice();

	m_vertexBuffer = new VulkanBuffer(*device, sizeof(vertices), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
	m_indexBuffer = new VulkanBuffer(*device, sizeof(indices), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
	m_uniformBuffer = new VulkanBuffer(*device, sizeof(uboVS), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
	//m_uniformMemory = device->allocateMemory(m_uniformBuffer->getSize());


	VkDeviceSize bufferSize = m_vertexBuffer->getSize();
	// We want to place the index buffer behind the vertex buffer. Need to take
	// the alignment into account to find the next suitable location
	VkDeviceSize indexBufferOffset = RoundToNextMultiple(bufferSize, m_indexBuffer->getAlignment());

	bufferSize = indexBufferOffset + m_indexBuffer->getSize();
	m_meshMemory = device->allocateMemory(m_vertexBuffer->getMemoryTypeBits(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,bufferSize);

	VulkanDeviceMemory deviceMemory(device->getDevice(), m_meshMemory);
	deviceMemory.map(0, VK_WHOLE_SIZE);
	deviceMemory.copyInto(vertices, 0, sizeof(vertices));
	deviceMemory.copyInto(indices, static_cast<uint32_t>(indexBufferOffset), sizeof(indices));
	deviceMemory.unmap();

	m_vertexBuffer->bindToMemory(m_meshMemory, 0);
	m_indexBuffer->bindToMemory(m_meshMemory, static_cast<uint32_t>(indexBufferOffset));
	
}

void TestScreen::createPipelineStateObject()
{
	VulkanDevice *device = getApplication()->getDevice();
	m_vertexShader = new VulkanShaderModule(device->getDevice(), BasicVertexShader, sizeof(BasicVertexShader));
	m_fragmentShader = new VulkanShaderModule(device->getDevice(), BasicFragmentShader, sizeof(BasicFragmentShader));

	Window *window = getApplication()->getWindow();
	
	m_pipelineLayout = new PipelineLayout(device->getDevice());
	m_graphicsPipeline = createPipeline(device->getDevice(), window->getRenderPass()->getHandle(), m_pipelineLayout->getHandle(), m_vertexShader->getHandle(), m_fragmentShader->getHandle(), VkExtent2D{window->getWidth(), window->getHeight() });
}