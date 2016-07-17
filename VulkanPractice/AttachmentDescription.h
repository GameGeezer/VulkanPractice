#pragma once

#include <vulkan\vulkan.h>

class AttachmentDescription
{
public:
	AttachmentDescription(VkSampleCountFlagBits sampleCount, VkFormat swapchainFormat, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout initialLayout, VkImageLayout finalLayout, bool aliased);

	~AttachmentDescription();

	VkAttachmentDescription*
	getDescription();

private:
	VkAttachmentDescription m_description = {};
};