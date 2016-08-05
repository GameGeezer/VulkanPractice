#include "TextureLoader.h"

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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

VulkanTexture2D*
TextureLoader::load(VulkanDevice &device, std::string fileName, VkFormat format)
{
	int32_t texWidth, texHeight, texChannels;
	stbi_uc* pixels = stbi_load(fileName.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
	VkDeviceSize imageSize = texWidth * texHeight * 4;

	int32_t mipMapLevels = 1;

	if (!pixels) {
		throw std::runtime_error("failed to load texture image!");
	}

	VkDevice rawDevice = device.getHandle();

	// Create the staging image and allocate memory
	VulkanImage2D stagingImage(rawDevice, format, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_TILING_LINEAR, VK_IMAGE_USAGE_TRANSFER_SRC_BIT, mipMapLevels, texWidth, texHeight);
	VkDeviceMemory stagingMemory = device.allocateMemory(stagingImage.getMemoryTypeBits(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingImage.getSize());
	VulkanDeviceMemory stagingMemoryWrapper(device.getHandle(), stagingMemory);
	stagingImage.bindToMemory(stagingMemoryWrapper, 0);

	void* data;
	vkMapMemory(rawDevice, stagingMemory, 0, imageSize, 0, &data);
	memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(rawDevice, stagingMemory);

	stbi_image_free(pixels);

	//	Create the texture image and allocate memory
	VulkanImage2D *image = new VulkanImage2D(rawDevice, format, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, mipMapLevels, texWidth, texHeight);
	VkDeviceMemory imageMemory = device.allocateMemory(image->getMemoryTypeBits(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, image->getSize());
	VulkanDeviceMemory *imageMemoryWrapper = new VulkanDeviceMemory(device.getHandle(), imageMemory);
	image->bindToMemory(*imageMemoryWrapper, 0);

	VulkanImageSubResourceRange subResourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, mipMapLevels, 0, 1);

	VkImageMemoryBarrier setSRCBarrier = stagingImage.createSetLayoutBarrier(VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, *(subResourceRange.getRaw()));
	VkImageMemoryBarrier setDSTBarrier = image->createSetLayoutBarrier(VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, *(subResourceRange.getRaw()));

	VulkanCommandBuffer oneTimeBuffer(device.beginSingleTimeCommands());
	oneTimeBuffer.addBarriers(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, nullptr, 0, nullptr, 0, &setSRCBarrier, 1);
	
	oneTimeBuffer.addBarriers(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, nullptr, 0, nullptr, 0, &setDSTBarrier, 1);
	oneTimeBuffer.commandCopyImage2D(texWidth, texHeight, stagingImage.getHandle(), 0, 0, image->getHandle(), 0, 0);

	VkImageMemoryBarrier setSampleableBarrier = image->createSetLayoutBarrier(VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, *(subResourceRange.getRaw()));

	oneTimeBuffer.addBarriers(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, nullptr, 0, nullptr, 0, &setSampleableBarrier, 1);
	device.endSingleTimeCommands(oneTimeBuffer.getHandle());


	//float maxLod = (useStaging) ? (float)mipLevels : 0.0f;

	// Create sampler
	VulkanSampler *sampler = new VulkanSampler(device, 0.0f);

	// Create image view
	// Textures are not directly accessed by the shaders and
	// are abstracted by image views containing additional
	// information and sub resource ranges
	VulkanImageSubResourceRange subRes(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);

	VulkanImageView *view = new VulkanImageView(device.getHandle(), image->getHandle(), format, VK_IMAGE_VIEW_TYPE_2D, *(subRes.getRaw()));

	return new VulkanTexture2D(*image, *imageMemoryWrapper, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, *view, *sampler);
}