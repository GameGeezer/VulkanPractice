#include "AttachmentDescription.h"

AttachmentDescription::AttachmentDescription(VkSampleCountFlagBits sampleCount, VkFormat swapchainFormat, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout initialLayout, VkImageLayout finalLayout, bool aliased)
{
	m_description.samples = sampleCount;
	m_description.format = swapchainFormat;			// Format of the image
	m_description.loadOp = loadOp;					// Should the image be cleared when the render pass starts
	m_description.storeOp = storeOp;					// Should the image be kept after the render pass?
	m_description.stencilLoadOp = stencilLoadOp;		// Should th stencil buffer be cleared
	m_description.stencilStoreOp = stencilStoreOp;	// Shuld the stencil buffer be kept
	m_description.initialLayout = initialLayout;
	m_description.finalLayout = finalLayout;
	m_description.flags = VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT * aliased;
}

AttachmentDescription::~AttachmentDescription()
{

}

VkAttachmentDescription*
AttachmentDescription::getDescription()
{
	return &m_description;
}