#include "FrameBuffer.h"

#include "RenderPass.h"

FrameBuffer::FrameBuffer(VkDevice device, RenderPass& renderPass, VkImageView &imageView, uint32_t width, uint32_t height) : m_device(device)
{
	VkFramebufferCreateInfo framebufferCreateInfo = {};
	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferCreateInfo.attachmentCount = 1;
	framebufferCreateInfo.pAttachments = &imageView;
	framebufferCreateInfo.height = height;
	framebufferCreateInfo.width = width;
	framebufferCreateInfo.layers = 1;
	framebufferCreateInfo.renderPass = renderPass.getHandle();

	vkCreateFramebuffer(m_device, &framebufferCreateInfo, nullptr, &m_frameBuffer);
}

FrameBuffer::~FrameBuffer()
{
	vkDestroyFramebuffer(m_device, m_frameBuffer, nullptr);
}

VkFramebuffer
FrameBuffer::getHandle()
{
	return m_frameBuffer;
}