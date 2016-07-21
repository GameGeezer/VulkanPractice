#pragma once

#include <vulkan\vulkan.h>

class VulkanFrameBuffer
{
public:
	VulkanFrameBuffer(VkDevice device, VkRenderPass renderPass, VkImageView &imageView, uint32_t width, uint32_t height);

	~VulkanFrameBuffer();

	VkFramebuffer
	getHandle();

private:
	VkDevice		m_device;
	VkFramebuffer	m_frameBuffer;
};