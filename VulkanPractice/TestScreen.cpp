#include "TestScreen.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
#include "VulkanStagedBuffer.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSet.h"

#include "Application.h"
#include "VulkanDevice.h"
#include "PipelineLayout.h"

#include "Player.h"

#include "Shaders.h"

#include "FileUtil.hpp"

#include "Utility.h"

#include "Camera.h"

#include <glm\mat4x4.hpp>

struct UniformBufferObject {
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

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

		m_descriptorPool = new VulkanDescriptorPool(getApplication()->getDevice()->getHandle());
		m_descriptorPool->addSize(1);
		m_descriptorPool->initialize(1);

		VulkanDescriptorSetLayout layout(getApplication()->getDevice()->getHandle());
		layout.addBinding(0, 1, VK_SHADER_STAGE_VERTEX_BIT);
		layout.initialize();

		m_descriptorSet = new VulkanDescriptorSet(getApplication()->getDevice()->getHandle());
		m_descriptorSet->addLayout(layout.getHandle());
		m_descriptorSet->initialize(m_descriptorPool->getHandle(), 1);

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = m_uniformBuffer->getBuffer();
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = m_descriptorSet->getHandle();
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(getApplication()->getDevice()->getHandle(), 1, &descriptorWrite, 0, nullptr);
		

	}

	m_player = new Player();
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
	m_player->update(delta);

	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count() / 1000.0f;

	UniformBufferObject ubo = {};
	ubo.model = glm::rotate(glm::mat4(), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = *(m_player->getCamera()->getView());
	ubo.proj = *(m_player->getCamera()->getProjection());
	ubo.proj[1][1] *= -1;
	VulkanCommandBuffer *commandBuffer = getApplication()->getCommandBuffers()->getCommandBufferAtIndex(1);

	m_uniformBuffer->stage(&ubo, 0, sizeof(ubo));
	m_uniformBuffer->update(*commandBuffer);
}

void
TestScreen::onRender(VulkanCommandBuffer *commandBufferWrapper)
{
	VkDescriptorSet destSet = m_descriptorSet->getHandle();
	VkCommandBuffer cmdBuf = getApplication()->getCommandBuffers()->getCommandBufferAtIndex(0)->getHandle();
	vkCmdBindDescriptorSets(cmdBuf, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout->getHandle(), 0, 1, &destSet, 0, nullptr);

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
		{ { -0.5f,  0.5f, 0 },{ 0, 0 } },
		// Upper Right
		{ { 0.5f,  0.5f, 0 },{ 1, 0 } },
		// Bottom right
		{ { 0.5f, -0.5f, 0 },{ 1, 1 } },
		// Bottom left
		{ { -0.5f, -0.5f, 0 },{ 0, 1 } }
	};

	static const int indices[6] =
	{
		0, 1, 2, 2, 3, 0
	};

	VulkanDevice *device = getApplication()->getDevice();

	m_vertexBuffer = new VulkanBuffer(*device, sizeof(vertices), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
	m_indexBuffer = new VulkanBuffer(*device, sizeof(indices), VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
	m_uniformBuffer = new VulkanStagedBuffer(*device, sizeof(UniformBufferObject), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);


	VkDeviceSize bufferSize = m_vertexBuffer->getSize();
	// We want to place the index buffer behind the vertex buffer. Need to take
	// the alignment into account to find the next suitable location
	VkDeviceSize indexBufferOffset = RoundToNextMultiple(bufferSize, m_indexBuffer->getAlignment());

	bufferSize = indexBufferOffset + m_indexBuffer->getSize();
	m_meshMemory = device->allocateMemory(m_vertexBuffer->getMemoryTypeBits(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,bufferSize);

	VulkanDeviceMemory deviceMemory(device->getHandle(), m_meshMemory);
	void *mapping = deviceMemory.map(0, VK_WHOLE_SIZE);
	deviceMemory.copyFrom(mapping, vertices, 0, sizeof(vertices));
	deviceMemory.copyFrom(mapping, indices, static_cast<uint32_t>(indexBufferOffset), sizeof(indices));
	deviceMemory.unmap();

	m_vertexBuffer->bindToMemory(m_meshMemory, 0);
	m_indexBuffer->bindToMemory(m_meshMemory, static_cast<uint32_t>(indexBufferOffset));
	
}

void TestScreen::createPipelineStateObject()
{
	VulkanDevice *device = getApplication()->getDevice();
	std::vector<char> vertShader = FileUtil::readFile("vert.spv");
	std::vector<char> fragShader = FileUtil::readFile("frag.spv");
	m_vertexShader = new VulkanShaderModule(device->getHandle(), vertShader.data(), static_cast<uint32_t>(vertShader.size()));
	//m_vertexShader = new VulkanShaderModule(device->getDevice(), BasicVertexShader, sizeof(BasicVertexShader));
	m_fragmentShader = new VulkanShaderModule(device->getHandle(), fragShader.data(), static_cast<uint32_t>(fragShader.size()));

	Window *window = getApplication()->getWindow();
	

	VulkanDescriptorSetLayout uniformLayout(device->getHandle());
	uniformLayout.addBinding(0, 1, VK_SHADER_STAGE_VERTEX_BIT);
	uniformLayout.initialize();

	m_pipelineLayout = new PipelineLayout(device->getHandle());
	m_pipelineLayout->addDescriptorSetLayout(uniformLayout.getHandle());
	m_pipelineLayout->initialize();

	m_graphicsPipeline = createPipeline(device->getHandle(), window->getRenderPass()->getHandle(), m_pipelineLayout->getHandle(), m_vertexShader->getHandle(), m_fragmentShader->getHandle(), VkExtent2D{window->getWidth(), window->getHeight() });
}