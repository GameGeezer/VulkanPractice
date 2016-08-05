#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanFrameBuffer
{
public:
	VulkanFrameBuffer(VkDevice device);

	~VulkanFrameBuffer();

	void
	initialize(VkRenderPass renderPass, uint32_t width, uint32_t height);

	void
	addAttachment(VkImageView imageView);

	VkFramebuffer
	getHandle();

private:
	VkDevice					m_device;
	VkFramebuffer				m_frameBuffer;
	std::vector<VkImageView>	m_attachments;

	bool						m_initialized = false;
};