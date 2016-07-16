#pragma once

#include <vulkan\vulkan.h>

#include <stdint.h>
#include <vector>

#include "VulkanColorAttachment.h"

class VulkanSubPassGroup
{
public:
	VulkanSubPassGroup();

	~VulkanSubPassGroup();

	void 
	addPass(std::vector<VkAttachmentReference>& colorAttachments, VkAttachmentReference depthReference);

	size_t 
	getPassCount();

	VkSubpassDescription*
	getAddress();

private:
	std::vector<VkSubpassDescription> m_subPasses;
};
