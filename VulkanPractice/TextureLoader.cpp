#include "TextureLoader.h"

#include <gli\gli.hpp>
#include <gli/texture2d.hpp>
#include <gli/convert.hpp>
#include <gli/load.hpp>

#include "Shared.h"

#include "VulkanStagedBuffer.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanImage.h"
#include "VulkanDeviceMemory.h"
#include "VulkanSampler.h"
#include "VulkanImageView.h"
#include "VulkanCommandBuffer.h"
#include "VulkanImageMemoryBarrier.h"
#include "VulkanImageView.h"
#include "VulkanImageSubResourceRange.h"
#include "VulkanDescriptorImageInfo.h"
#include "VulkanBuffer.h"
#include "VulkanImage2D.h"
#include "VulkanTexture2D.h"

#include <vector>

VulkanTexture2D*
TextureLoader::load(VulkanDevice &device, std::string fileName, VkFormat format, bool forceLinearTiling, VulkanCommandBuffer &commandBuffer, VkFence copyFence, uint64_t timeout)
{
	VkDevice rawDevice = device.getHandle();

	gli::texture loadedTexture = gli::load(fileName);
	gli::texture2d tex2D(loadedTexture);

	assert(!tex2D.empty());

	uint32_t width = static_cast<uint32_t>(tex2D[0].extent().x);
	uint32_t height = static_cast<uint32_t>(tex2D[0].extent().y);
	uint32_t mipLevels = static_cast<uint32_t>(tex2D.levels());

	// Create the staging image and allocate memory
	VulkanImage2D stagingImage(rawDevice, format, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT, mipLevels, width, height);
	VkDeviceMemory stagingMemory = device.allocateMemory(stagingImage.getMemoryTypeBits(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingImage.getSize());
	VulkanDeviceMemory stagingMemoryWrapper(device.getHandle(), stagingMemory);
	stagingImage.bindToMemory(stagingMemoryWrapper, 0);
	//stagingImage.setMemory(tex2D.data(), 0, tex2D.size());

	void* data;
	vkMapMemory(rawDevice, stagingMemory, 0, stagingImage.getSize(), 0, &data);
	memcpy(data, tex2D.data(), stagingImage.getSize());
	vkUnmapMemory(rawDevice, stagingMemory);

	//	Create the texture image and allocate memory
	VulkanImage2D *image = new VulkanImage2D(rawDevice, format, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, mipLevels, width, height);
	VkDeviceMemory imageMemory = device.allocateMemory(image->getMemoryTypeBits(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image->getSize());
	VulkanDeviceMemory *imageMemoryWrapper = new VulkanDeviceMemory(device.getHandle(), imageMemory);
	image->bindToMemory(*imageMemoryWrapper, 0);

	VulkanImageSubResourceRange subResourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, mipLevels, 0, 1);

	VkImageMemoryBarrier setSRCBarrier = stagingImage.createSetLayoutBarrier(VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, *(subResourceRange.getRaw()));
	VkImageMemoryBarrier setDSTBarrier = image->createSetLayoutBarrier(VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, *(subResourceRange.getRaw()));

	commandBuffer.begin(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

	commandBuffer.addBarriers(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, nullptr, 0, nullptr, 0, &setSRCBarrier, 1);
	commandBuffer.addBarriers(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, nullptr, 0, nullptr, 0, &setDSTBarrier, 1);

	commandBuffer.commandCopyImage2D(width, height, stagingImage.getHandle(), 0, 0, image->getHandle(), 0, 0);

	VkImageMemoryBarrier setSampleableBarrier = image->createSetLayoutBarrier(VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, *(subResourceRange.getRaw()));
	commandBuffer.addBarriers(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, nullptr, 0, nullptr, 0, &setSampleableBarrier, 1);

	commandBuffer.end();

	//float maxLod = (useStaging) ? (float)mipLevels : 0.0f;

	// Create sampler
	VulkanSampler *sampler = new VulkanSampler(device, 0.0f);

	// Create image view
	// Textures are not directly accessed by the shaders and
	// are abstracted by image views containing additional
	// information and sub resource ranges
	VulkanImageSubResourceRange subRes(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);

	VulkanImageView *view = new VulkanImageView(device.getHandle(), image->getHandle(), format, VK_IMAGE_VIEW_TYPE_2D, *(subRes.getRaw()));

	return new VulkanTexture2D(*image, *imageMemoryWrapper, VK_IMAGE_LAYOUT_GENERAL, *view, *sampler);
	/*
	

	// Get device properites for the requested texture format
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(device.getPhysicalDevice()->getHandle(), format, &formatProperties);

	// Only use linear tiling if requested (and supported by the device)
	// Support for linear tiling is mostly limited, so prefer to use
	// optimal tiling instead
	// On most implementations linear tiling will only support a very
	// limited amount of formats and features (mip maps, cubemaps, arrays, etc.)
	VkBool32 useStaging = !forceLinearTiling;

	if (forceLinearTiling)
	{
		// Don't use linear if format is not supported for (linear) shader sampling
		useStaging = !(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
	}

	VulkanImage *image = nullptr;
	VulkanDeviceMemory *imageMemoryWrapper = nullptr;
	// Create a host-visible staging buffer that contains the raw image data
	if (useStaging)
	{
		
		// Create a host-visible staging buffer that contains the raw image data
		
		VulkanBuffer stagingBuffer(device, static_cast<uint32_t>(tex2D.size()), VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_SHARING_MODE_EXCLUSIVE);
		VkDeviceMemory stagingMemory = device.allocateMemory(stagingBuffer.getMemoryTypeBits(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer.getSize());
		VulkanDeviceMemory stagingMemoryWrapper(device.getHandle(), stagingMemory); 
		stagingBuffer.bindToMemory(stagingMemory, 0);
		stagingBuffer.setMemory(tex2D.data(), 0, tex2D.size());

		// Copy texture data into staging buffer
		stagingBuffer.setMemory(tex2D.data(), 0, tex2D.size());

		// Setup buffer copy regions for each mip level
		std::vector<VkBufferImageCopy> bufferCopyRegions;
		uint32_t offset = 0;
		
		for (uint32_t i = 0; i < mipLevels; i++)
		{
			VkBufferImageCopy bufferCopyRegion = {};
			bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bufferCopyRegion.imageSubresource.mipLevel = i;
			bufferCopyRegion.imageSubresource.baseArrayLayer = 0;
			bufferCopyRegion.imageSubresource.layerCount = 1;
			bufferCopyRegion.imageExtent.width = width;
			bufferCopyRegion.imageExtent.height = height;
			bufferCopyRegion.imageExtent.depth = 1;
			bufferCopyRegion.bufferOffset = offset;

			bufferCopyRegions.push_back(bufferCopyRegion);

			offset += static_cast<uint32_t>(tex2D[i].size());
		}

		VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		// Create optimal tiled target image
		image = new VulkanImage2D(device.getHandle(), format, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_TILING_OPTIMAL, usage, mipLevels, width, height);
		
		//	Grab memory requirements
		uint32_t memoryType = image->getMemoryRequirements().memoryTypeBits;
		uint32_t memorySize = static_cast<uint32_t>(image->getMemoryRequirements().size);
		//	Allocate memory
		VkDeviceMemory imageMemory = device.allocateMemory(memoryType, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memorySize);
		imageMemoryWrapper = new VulkanDeviceMemory(device.getHandle(), imageMemory);
		image->bindToMemory(imageMemory, 0);

		VulkanImageSubResourceRange subResourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, mipLevels, 0, 1);
		
		// Image barrier for optimal image (target)
		// Optimal image will be used as destination for the copy
		VkImageMemoryBarrier setDSTBarrier = image->createSetLayoutBarrier(VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, *(subResourceRange.getRaw()));

		commandBuffer.begin(0);

		commandBuffer.addBarriers(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, nullptr, 0, nullptr, 0, &setDSTBarrier, 1);

		// Copy mip levels from staging buffer
		//image->copyFromBuffer(commandBuffer.getHandle(), stagingBuffer.getHandle(), bufferCopyRegions);

		// Change texture image layout to shader read after all mip levels have been copied
		VkImageMemoryBarrier setReadOnlyBarrier = image->createSetLayoutBarrier(VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, *(subResourceRange.getRaw()));
		commandBuffer.addBarriers(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, nullptr, 0, nullptr, 0, &setReadOnlyBarrier, 1);

		// Submit command buffer containing copy and image layout commands
		commandBuffer.end();
		
		// Create a fence to make sure that the copies have finished before continuing
		VkCommandBuffer rawCommandBufferHandle = commandBuffer.getHandle();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &rawCommandBufferHandle;

		VkFence copyImageFence;
		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = 0;
		ErrorCheck(vkCreateFence(device.getHandle(), &fenceCreateInfo, nullptr, &copyImageFence));

		device.submitToQueue(1, &submitInfo, copyImageFence);
		
		ErrorCheck(vkWaitForFences(device.getHandle(), 1, &copyFence, VK_TRUE, timeout));
		vkDestroyFence(device.getHandle(), copyImageFence, nullptr);

		stagingMemoryWrapper.release();
		
	}
	else
	{
		// Prefer using optimal tiling, as linear tiling 
		// may support only a small set of features 
		// depending on implementation (e.g. no mip maps, only one layer, etc.)

		// Check if this support is supported for linear tiling
		assert(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);

		image = new VulkanImage2D(device.getHandle(), format, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_SAMPLED_BIT, mipLevels, width, height);

		//	Grab memory requirements
		uint32_t memoryType = image->getMemoryRequirements().memoryTypeBits;
		uint32_t memorySize = static_cast<uint32_t>(image->getMemoryRequirements().size);
		//	Allocate memory
		VkDeviceMemory imageMemory = device.allocateMemory(memoryType, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memorySize);
		imageMemoryWrapper = new VulkanDeviceMemory(device.getHandle(), imageMemory);
		image->bindToMemory(imageMemory, 0);

		// Get sub resource layout
		// Mip map count, array layer, etc.
		VkImageSubresource subRes = {};
		subRes.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		subRes.mipLevel = 0;

		VkSubresourceLayout subResLayout;
		image->getSubresourceLayout(subRes, subResLayout);

		// Map image memory
		void *mapping = imageMemoryWrapper->map(0, image->getMemoryRequirements().size);
		imageMemoryWrapper->copyFrom(mapping, tex2D[subRes.mipLevel].data(), 0, tex2D[subRes.mipLevel].size());

		imageMemoryWrapper->unmap();

		VulkanImageSubResourceRange imagesubResourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
		// Change texture image layout to shader read after all mip levels have been copied
		VkImageMemoryBarrier setReadOnlyBarrier = image->createSetLayoutBarrier(VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, *(imagesubResourceRange.getRaw()));
		commandBuffer.begin(0);
		commandBuffer.addBarriers(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, nullptr, 0, nullptr, 0, &setReadOnlyBarrier, 1);

		// Submit command buffer containing copy and image layout commands
		commandBuffer.end();

		VkFence nullFence = { VK_NULL_HANDLE };
		// Create a fence to make sure that the copies have finished before continuing
		VkCommandBuffer rawCommandBufferHandle = commandBuffer.getHandle();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &rawCommandBufferHandle;

		device.submitToQueue(1, &submitInfo, copyFence);
		device.waitOnQueue();
	}

	
	float maxLod = (useStaging) ? (float)mipLevels : 0.0f;

	
	// Create sampler
	VulkanSampler *sampler = new VulkanSampler(device, maxLod);

	// Create image view
	// Textures are not directly accessed by the shaders and
	// are abstracted by image views containing additional
	// information and sub resource ranges
	VulkanImageSubResourceRange subRes(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
	VulkanImageView *view = new VulkanImageView(device.getHandle(), image->getHandle(), format, VK_IMAGE_VIEW_TYPE_2D, *(subRes.getRaw()));

	return new VulkanTexture2D(*image, *imageMemoryWrapper, VK_IMAGE_LAYOUT_GENERAL, *view, *sampler);
	*/
}