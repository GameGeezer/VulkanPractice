#include "VulkanSampler.h"

#include "Shared.h"

#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"


VulkanSampler::VulkanSampler(VulkanDevice &device, float maxLod) : m_device(&device)
{
	VkSamplerCreateInfo sampler = {};
	sampler.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
	sampler.magFilter = VK_FILTER_LINEAR;
	sampler.minFilter = VK_FILTER_LINEAR;
	sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler.mipLodBias = 0.0f;
	sampler.compareOp = VK_COMPARE_OP_NEVER;
	sampler.minLod = 0.0f;
	// Set max level-of-detail to mip level count of the texture
	sampler.maxLod = maxLod;
	// Enable anisotropic filtering
	// This feature is optional, so we must check if it's supported on the device
	VkBool32 samplerAnisotropy = m_device->getPhysicalDevice()->getFeatures()->samplerAnisotropy;

	if (samplerAnisotropy)
	{
		// Use max. level of anisotropy for this example
		sampler.maxAnisotropy = m_device->getPhysicalDevice()->getProperties()->limits.maxSamplerAnisotropy;
		sampler.anisotropyEnable = VK_TRUE;
	}
	else
	{
		// The device does not support anisotropic filtering
		sampler.maxAnisotropy = 1.0;
		sampler.anisotropyEnable = VK_FALSE;
	}

	sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	ErrorCheck(vkCreateSampler(m_device->getHandle(), &sampler, nullptr, &m_sampler));
	
}

VulkanSampler::~VulkanSampler()
{
	vkDestroySampler(m_device->getHandle(), m_sampler, nullptr);
}

VkSampler
VulkanSampler::getHandle()
{
	return m_sampler;
}