#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanRenderSubPass;

class VulkanRenderPass
{
public:
	VulkanRenderPass(VkDevice device);

	~VulkanRenderPass();

	void
	begin(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, uint32_t width, uint32_t height, VkClearValue *clearValue, uint32_t clearCount);

	void
	end(VkCommandBuffer commandBuffer);

	void
	initialize();

	void
	addSubPass(VulkanRenderSubPass& subPass);

	void
	addAttachmentDescription(VkAttachmentDescription attachment);

	VkRenderPass
	getHandle();

private:
	VkDevice								m_device;
	VkRenderPass							m_renderPass;
	std::vector<VkSubpassDescription>		m_subpasses;
	std::vector<VkAttachmentDescription>	m_attachmentDescriptions;
	bool									m_initialized = false;
};