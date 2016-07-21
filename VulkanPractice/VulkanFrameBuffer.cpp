#include "VulkanFrameBuffer.h"

VulkanFrameBuffer::VulkanFrameBuffer(VkDevice device, VkRenderPass renderPass, VkImageView &imageView, uint32_t width, uint32_t height) : m_device(device)
{
	VkFramebufferCreateInfo framebufferCreateInfo = {};
	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferCreateInfo.attachmentCount = 1;
	framebufferCreateInfo.pAttachments = &imageView;
	framebufferCreateInfo.height = height;
	framebufferCreateInfo.width = width;
	framebufferCreateInfo.layers = 1;
	framebufferCreateInfo.renderPass = renderPass;

	vkCreateFramebuffer(m_device, &framebufferCreateInfo, nullptr, &m_frameBuffer);
}

VulkanFrameBuffer::~VulkanFrameBuffer()
{
	vkDestroyFramebuffer(m_device, m_frameBuffer, nullptr);
}

VkFramebuffer
VulkanFrameBuffer::getHandle()
{
	return m_frameBuffer;
}