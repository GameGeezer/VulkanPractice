#pragma once

#include "Screen.h"

#include <vulkan\vulkan.h>

#include <stdint.h>

class VulkanDevice;
class VulkanInstance;
class VulkanCommandPool;
class VulkanCommandBufferGroup;
class VulkanFenceGroup;
class Window;
class GraphicsPipeline;
class VulkanBuffer;
class VulkanShaderModule;
class PipelineLayout;
class VulkanStagedBuffer;
class VulkanDescriptorPool;
class VulkanDescriptorSet;
class Player;

class TestScreen : public Screen
{
public:
	void
	onCreate() override;

	void
	onPause() override;

	void
	onLeave() override;

	void
	onResume() override;

	void
	onUpdate(uint32_t delta) override;

	void
	onRender(VulkanCommandBuffer *commandBufferWrapper) override;

	void
	onResize(uint32_t width, uint32_t height) override;

	void
	onDestroy() override;

private:

	GraphicsPipeline* TestScreen::
	createPipeline(VkDevice device, VkRenderPass renderPass, VkPipelineLayout layout, VkShaderModule vertexShader, VkShaderModule fragmentShader, VkExtent2D viewportSize);

	void 
	createPipelineStateObject();

	void
	createMeshBuffers(VkCommandBuffer uploadCommandBuffer);

	GraphicsPipeline		*	m_graphicsPipeline;
	PipelineLayout			*	m_pipelineLayout;

	VulkanBuffer			*	m_vertexBuffer;
	VulkanBuffer			*	m_indexBuffer;

	VulkanStagedBuffer		*	m_uniformBuffer;

	VulkanShaderModule		*	m_vertexShader;
	VulkanShaderModule		*	m_fragmentShader;

	VkDeviceMemory				m_meshMemory;

	VulkanDescriptorPool	*	m_descriptorPool;
	VulkanDescriptorSet		*	m_descriptorSet;

	Player					*	m_player;

	uint32_t					m_indicesCount; // delete
};
