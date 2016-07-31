#pragma once

#include <vulkan\vulkan.h>

class VulkanDevice;

class VulkanSampler
{
public:
	VulkanSampler(VulkanDevice &device, uint32_t mipMapLevels);

	~VulkanSampler();

private:
	VulkanDevice	*	m_device;
	VkSampler			m_sampler;
};