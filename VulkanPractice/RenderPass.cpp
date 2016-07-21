#include "RenderPass.h"

#include "RenderSubPass.h"
#include "AttachmentDescription.h"

RenderPass::RenderPass(VkDevice device) : m_device(device)
{
	
}

RenderPass::~RenderPass()
{
	vkDestroyRenderPass(m_device, m_renderPass, nullptr);
}

void
RenderPass::begin(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, uint32_t width, uint32_t height, VkClearValue &clearValue, uint32_t clearCount)
{
	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.framebuffer = frameBuffer;
	renderPassBeginInfo.renderArea.extent.width = width;
	renderPassBeginInfo.renderArea.extent.height = height;
	renderPassBeginInfo.renderPass = m_renderPass;
	renderPassBeginInfo.pClearValues = &clearValue;
	renderPassBeginInfo.clearValueCount = clearCount;

	vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void
RenderPass::end(VkCommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}

void
RenderPass::initialize()
{
	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = m_attachmentDescriptions.size();
	renderPassCreateInfo.subpassCount = m_subpasses.size();
	renderPassCreateInfo.pSubpasses = m_subpasses.data();
	renderPassCreateInfo.pAttachments = m_attachmentDescriptions.data();

	vkCreateRenderPass(m_device, &renderPassCreateInfo, nullptr, &m_renderPass);
}

void
RenderPass::addSubPass(RenderSubPass& subPass)
{
	m_subpasses.push_back(*(subPass.getDescription()));
}


void
RenderPass::addAttachmentDescription(AttachmentDescription& attachment)
{
	m_attachmentDescriptions.push_back(*(attachment.getDescription()));
}

VkRenderPass
RenderPass::getHandle()
{
	return m_renderPass;
}