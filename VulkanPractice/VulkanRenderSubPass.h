#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanRenderSubPass
{
public:
	VulkanRenderSubPass();

	~VulkanRenderSubPass();

	void
	initialize();

	void
	addColorAttachment(uint32_t attachmentIndex, VkImageLayout layout);

	VkSubpassDescription*
	getDescription();

private:
	VkSubpassDescription				m_description = {};;
	std::vector<VkAttachmentReference>	m_colorAttachments;
};