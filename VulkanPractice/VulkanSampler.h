#pragma once

#include <vulkan\vulkan.h>

class VulkanDevice;

class VulkanSampler
{
public:
	VulkanSampler(VulkanDevice &device, float maxLod);

	~VulkanSampler();

	VkSampler
	getHandle();

private:
	VulkanDevice	*	m_device;
	VkSampler			m_sampler;
};