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
	sampler.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	sampler.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
	sampler.unnormalizedCoordinates = VK_FALSE;
	sampler.compareEnable = VK_FALSE;
	sampler.compareOp = VK_COMPARE_OP_ALWAYS;
	sampler.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
	sampler.mipLodBias = 0.0f;
	sampler.minLod = 0.0f;
	sampler.maxLod = 0.0f;

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