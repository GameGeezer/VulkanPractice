#include "VulkanRenderPass.h"

#include "VulkanRenderSubPass.h"

VulkanRenderPass::VulkanRenderPass(VkDevice device) : m_device(device)
{
	
}

VulkanRenderPass::~VulkanRenderPass()
{
	vkDestroyRenderPass(m_device, m_renderPass, nullptr);
}

void
VulkanRenderPass::begin(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, uint32_t width, uint32_t height, VkClearValue &clearValue, uint32_t clearCount)
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
VulkanRenderPass::end(VkCommandBuffer commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer);
}

void
VulkanRenderPass::initialize()
{
	if (m_initialized)
	{
		throw "VulkanRenderPass has already been initialized";
	}

	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = static_cast<uint32_t> (m_attachmentDescriptions.size());
	renderPassCreateInfo.subpassCount = static_cast<uint32_t> (m_subpasses.size());
	renderPassCreateInfo.pSubpasses = m_subpasses.data();
	renderPassCreateInfo.pAttachments = m_attachmentDescriptions.data();

	vkCreateRenderPass(m_device, &renderPassCreateInfo, nullptr, &m_renderPass);

	m_initialized = true;
}

void
VulkanRenderPass::addSubPass(VulkanRenderSubPass& subPass)
{
	m_subpasses.push_back(*(subPass.getDescription()));
}


void
VulkanRenderPass::addAttachmentDescription(VkAttachmentDescription attachment)
{
	m_attachmentDescriptions.push_back(attachment);
}

VkRenderPass
VulkanRenderPass::getHandle()
{
	return m_renderPass;
}