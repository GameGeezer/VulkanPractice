#include "RenderSubPass.h"

RenderSubPass::RenderSubPass()
{

}

RenderSubPass::~RenderSubPass()
{

}

void
RenderSubPass::initialize()
{
	m_description.inputAttachmentCount = 0;
	m_description.pColorAttachments = m_colorAttachments.data();
	m_description.colorAttachmentCount = m_colorAttachments.size();
	m_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
}

void
RenderSubPass::addColorAttachment(uint32_t attachmentIndex, VkImageLayout layout)
{
	VkAttachmentReference attachmentReference = {};
	attachmentReference.attachment = 0;
	attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	m_colorAttachments.push_back(attachmentReference);
}

VkSubpassDescription*
RenderSubPass::getDescription()
{
	return &m_description;
}