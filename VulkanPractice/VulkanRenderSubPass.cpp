#include "VulkanRenderSubPass.h"

VulkanRenderSubPass::VulkanRenderSubPass()
{

}

VulkanRenderSubPass::~VulkanRenderSubPass()
{

}

void
VulkanRenderSubPass::initialize()
{
	m_description.inputAttachmentCount = 0;
	m_description.pColorAttachments = m_colorAttachments.data();
	m_description.colorAttachmentCount = static_cast<uint32_t>(m_colorAttachments.size());
	m_description.pDepthStencilAttachment = &m_depthStencilAttachment;
	m_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
}

void
VulkanRenderSubPass::addColorAttachment(uint32_t attachmentIndex, VkImageLayout layout)
{
	VkAttachmentReference attachmentReference = {};
	attachmentReference.attachment = attachmentIndex;
	attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	m_colorAttachments.push_back(attachmentReference);
}

void
VulkanRenderSubPass::setDepthStencilAttachment(uint32_t attachmentIndex, VkImageLayout layout)
{
	m_depthStencilAttachment.attachment = attachmentIndex;
	m_depthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
}

VkSubpassDescription*
VulkanRenderSubPass::getDescription()
{
	return &m_description;
}