#include "TextureLoader.h"

#include <gli\gli.hpp>

#include "VulkanStagedBuffer.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"
#include "VulkanDeviceMemory.h"

void
TextureLoader::load(VulkanDevice &device, std::string fileName, VkFormat format)
{
	/*
	gli::texture2d tex2D(gli::load(fileName));

	assert(!tex2D.empty());

	VkFormatProperties formatProperties;

	texture.width = static_cast<uint32_t>(tex2D[0].dimensions().x);
	texture.height = static_cast<uint32_t>(tex2D[0].dimensions().y);
	texture.mipLevels = static_cast<uint32_t>(tex2D.levels());

	// Get device properites for the requested texture format
	vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

	// Only use linear tiling if requested (and supported by the device)
	// Support for linear tiling is mostly limited, so prefer to use
	// optimal tiling instead
	// On most implementations linear tiling will only support a very
	// limited amount of formats and features (mip maps, cubemaps, arrays, etc.)
	VkBool32 useStaging = true;

	// Only use linear tiling if forced
	if (forceLinearTiling)
	{
		// Don't use linear if format is not supported for (linear) shader sampling
		useStaging = !(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
	}

	VkMemoryAllocateInfo memAllocInfo = vkTools::initializers::memoryAllocateInfo();
	VkMemoryRequirements memReqs = {};

	// Create a host-visible staging buffer that contains the raw image data
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingMemory;

	VulkanStagedBuffer stagedBuffer(device, tex2D.size(), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);

	stagedBuffer.stage(tex2D.data(), tex2D.size());

	// Setup buffer copy regions for each mip level
	std::vector<VkBufferImageCopy> bufferCopyRegions;
	uint32_t offset = 0;

	for (uint32_t i = 0; i < texture.mipLevels; i++)
	{
		VkBufferImageCopy bufferCopyRegion = {};
		bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		bufferCopyRegion.imageSubresource.mipLevel = i;
		bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
		bufferCopyRegion.imageSubresource.layerCount = 1;
		bufferCopyRegion.imageExtent.width = static_cast<uint32_t>(tex2D[i].dimensions().x);
		bufferCopyRegion.imageExtent.height = static_cast<uint32_t>(tex2D[i].dimensions().y);
		bufferCopyRegion.imageExtent.depth = 1;
		bufferCopyRegion.bufferOffset = offset;

		bufferCopyRegions.push_back(bufferCopyRegion);

		offset += static_cast<uint32_t>(tex2D[i].size());
	}

	VulkanImage image(device.getDevice(), format, texture.mipLevels, width, height);
	uint32_t memoryType = image.getMemoryRequirements().memoryTypeBits;
	uint32_t memorySize = image.getMemoryRequirements().size;
	VkDeviceMemory imageMemory = device.allocateMemory(memoryType, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memorySize);
	VulkanDeviceMemory imageMemoryWrapper(device.getDevice(), imageMemory);
	image.bindToMemory(imageMemoryWrapper.getHandle(), 0);

	VkCommandBuffer copyCmd = VulkanExampleBase::createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);

	// Image barrier for optimal image

	// The sub resource range describes the regions of the image we will be transition
	VkImageSubresourceRange subresourceRange = {};
	// Image only contains color data
	subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	// Start at first mip level
	subresourceRange.baseMipLevel = 0;
	// We will transition on all mip levels
	subresourceRange.levelCount = texture.mipLevels;
	// The 2D texture only has one layer
	subresourceRange.layerCount = 1;

	// Optimal image will be used as destination for the copy, so we must transfer from our
	// initial undefined image layout to the transfer destination layout
	setImageLayout(
		copyCmd,
		texture.image,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_LAYOUT_UNDEFINED,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		subresourceRange);

	// Copy mip levels from staging buffer
	vkCmdCopyBufferToImage(
		copyCmd,
		stagingBuffer,
		texture.image,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		static_cast<uint32_t>(bufferCopyRegions.size()),
		bufferCopyRegions.data());

	// Change texture image layout to shader read after all mip levels have been copied
	texture.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	setImageLayout(
		copyCmd,
		texture.image,
		VK_IMAGE_ASPECT_COLOR_BIT,
		VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		texture.imageLayout,
		subresourceRange);

	VulkanExampleBase::flushCommandBuffer(copyCmd, queue, true);

	// Clean up staging resources
	vkFreeMemory(device, stagingMemory, nullptr);
	vkDestroyBuffer(device, stagingBuffer, nullptr);

	//////////////////////////////////////////////////////////////////

	// Create sampler
	// In Vulkan textures are accessed by samplers
	// This separates all the sampling information from the 
	// texture data
	// This means you could have multiple sampler objects
	// for the same texture with different settings
	// Similar to the samplers available with OpenGL 3.3
	VkSamplerCreateInfo sampler = vkTools::initializers::samplerCreateInfo();
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
	sampler.maxLod = (useStaging) ? (float)texture.mipLevels : 0.0f;
	// Enable anisotropic filtering
	// This feature is optional, so we must check if it's supported on the device
	if (vulkanDevice->features.samplerAnisotropy)
	{
		// Use max. level of anisotropy for this example
		sampler.maxAnisotropy = vulkanDevice->properties.limits.maxSamplerAnisotropy;
		sampler.anisotropyEnable = VK_TRUE;
	}
	else
	{
		// The device does not support anisotropic filtering
		sampler.maxAnisotropy = 1.0;
		sampler.anisotropyEnable = VK_FALSE;
	}
	sampler.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
	VK_CHECK_RESULT(vkCreateSampler(device, &sampler, nullptr, &texture.sampler));

	// Create image view
	// Textures are not directly accessed by the shaders and
	// are abstracted by image views containing additional
	// information and sub resource ranges
	VkImageViewCreateInfo view = vkTools::initializers::imageViewCreateInfo();
	view.image = VK_NULL_HANDLE;
	view.viewType = VK_IMAGE_VIEW_TYPE_2D;
	view.format = format;
	view.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
	// The subresource range describes the set of mip levels (and array layers) that can be accessed through this image view
	// It's possible to create multiple image views for a single image referring to different (and/or overlapping) ranges of the image
	view.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	view.subresourceRange.baseMipLevel = 0;
	view.subresourceRange.baseArrayLayer = 0;
	view.subresourceRange.layerCount = 1;
	// Linear tiling usually won't support mip maps
	// Only set mip map count if optimal tiling is used
	view.subresourceRange.levelCount = (useStaging) ? texture.mipLevels : 1;
	view.image = texture.image;
	VK_CHECK_RESULT(vkCreateImageView(device, &view, nullptr, &texture.view));

	// Fill image descriptor image info that can be used during the descriptor set setup
	texture.descriptor.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
	texture.descriptor.imageView = texture.view;
	texture.descriptor.sampler = texture.sampler;
	*/
}