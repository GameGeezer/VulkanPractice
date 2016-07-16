#include "VulkanRenderPass.h"

#include "Shared.h"

VulkanRenderPass::VulkanRenderPass(VkDevice device, VulkanSubPassGroup subPasses, std::vector<VkAttachmentDescription> attachments)
{
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	// Set attachments used in our renderpass
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	// We only use one subpass
	renderPassInfo.subpassCount = static_cast<uint32_t>(subPasses.getPassCount());;
	renderPassInfo.pSubpasses = subPasses.getAddress();
	// As we only use one subpass we don't have any subpass dependencies
	renderPassInfo.dependencyCount = 0;
	renderPassInfo.pDependencies = nullptr;

	// Create the renderpass
	ErrorCheck(vkCreateRenderPass(device, &renderPassInfo, nullptr, &m_renderPass));
}

VulkanRenderPass::~VulkanRenderPass()
{

}
