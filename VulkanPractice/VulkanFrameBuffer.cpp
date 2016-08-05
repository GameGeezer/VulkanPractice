#include "VulkanFrameBuffer.h"

VulkanFrameBuffer::VulkanFrameBuffer(VkDevice device) : m_device(device)
{

}

void
VulkanFrameBuffer::initialize(VkRenderPass renderPass, uint32_t width, uint32_t height)
{
	if (m_initialized)
	{
		throw "Frame buffer is already initialized";
	}

	m_initialized = true;

	VkFramebufferCreateInfo framebufferCreateInfo = {};
	framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferCreateInfo.attachmentCount = static_cast<uint32_t>(m_attachments.size());
	framebufferCreateInfo.pAttachments = m_attachments.data();
	framebufferCreateInfo.height = height;
	framebufferCreateInfo.width = width;
	framebufferCreateInfo.layers = 1;
	framebufferCreateInfo.renderPass = renderPass;

	vkCreateFramebuffer(m_device, &framebufferCreateInfo, nullptr, &m_frameBuffer);
}


void
VulkanFrameBuffer::addAttachment(VkImageView imageView)
{
	m_attachments.push_back(imageView);
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