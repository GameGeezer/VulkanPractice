#include "VulkanSubPassGroup.h"

VulkanSubPassGroup::VulkanSubPassGroup()
{

}

VulkanSubPassGroup::~VulkanSubPassGroup()
{

}

void 
VulkanSubPassGroup::addPass(std::vector<VkAttachmentReference>& colorAttachments, VkAttachmentReference depthReference)
{
	VkSubpassDescription subpass = {};

	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	// Input attachments can be used to sample from contents of attachments 
	// written to by previous sub passes in a setup with multiple sub passes
	// This example doesn't make use of this
	subpass.inputAttachmentCount = 0;
	subpass.pInputAttachments = nullptr;
	// Preserved attachments can be used to loop (and preserve) attachments
	// through a sub pass that does not actually use them
	// This example doesn't make use of this
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments = nullptr;
	// Resoluve attachments are resolved at the end of a sub pass and can be
	// used for e.g. multi sampling
	// This example doesn't make use of this
	subpass.pResolveAttachments = nullptr;
	// Reference to the color attachment
	subpass.colorAttachmentCount = colorAttachments.size();
	subpass.pColorAttachments = &colorAttachments.front();
	// Reference to the depth attachment
	subpass.pDepthStencilAttachment = &depthReference;

	m_subPasses.push_back(subpass);
}

size_t
VulkanSubPassGroup::getPassCount()
{
	return m_subPasses.size();
}

VkSubpassDescription*
VulkanSubPassGroup::getAddress()
{
	return m_subPasses.data();
}