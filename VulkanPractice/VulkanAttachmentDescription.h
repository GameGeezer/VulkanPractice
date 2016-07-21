#pragma once

#include <vulkan\vulkan.h>

class VulkanAttachmentDescription
{
public:
	VulkanAttachmentDescription(VkSampleCountFlagBits sampleCount, VkFormat swapchainFormat, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout initialLayout, VkImageLayout finalLayout, bool aliased);

	~VulkanAttachmentDescription();

	VkAttachmentDescription*
	getDescription();

private:
	VkAttachmentDescription m_description = {};
};