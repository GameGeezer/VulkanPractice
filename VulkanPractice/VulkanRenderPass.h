#pragma once

#include <vulkan\vulkan.h>

#include <stdint.h>

#include <vector>

#include "VulkanSubPassGroup.h"

class VulkanRenderPass
{
public:
	VulkanRenderPass(VkDevice device, VulkanSubPassGroup subPasses, std::vector<VkAttachmentDescription> attachments);

	~VulkanRenderPass();

private:

	VkRenderPass m_renderPass;
};
