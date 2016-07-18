#pragma once

#include <vulkan\vulkan.h>

class RenderPass;

class FrameBuffer
{
public:
	FrameBuffer(VkDevice device, RenderPass &renderPass, VkImageView &imageView, uint32_t width, uint32_t height);

	~FrameBuffer();

	VkFramebuffer
	getHandle();

private:
	VkDevice		m_device;
	VkFramebuffer	m_frameBuffer;
};