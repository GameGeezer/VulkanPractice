#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class RenderSubPass;
class AttachmentDescription;

class RenderPass
{
public:
	RenderPass(VkDevice device);

	~RenderPass();

	void
	begin(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, uint32_t width, uint32_t height, VkClearValue &clearValue, uint32_t clearCount);

	void
	end(VkCommandBuffer commandBuffer);

	void
	initialize();

	void
	addSubPass(RenderSubPass& subPass);

	void
	addAttachmentDescription(AttachmentDescription& attachment);

	VkRenderPass
	getHandle();

private:
	VkDevice								m_device;
	VkRenderPass							m_renderPass;
	std::vector<VkSubpassDescription>		m_subpasses;
	std::vector<VkAttachmentDescription>	m_attachmentDescriptions;
};