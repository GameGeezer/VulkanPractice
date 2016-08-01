#include "TextureLoader.h"

#include <gli\gli.hpp>

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

void
TextureLoader::load(VulkanDevice &device, std::string fileName, VkFormat format, bool forceLinearTiling, VulkanCommandBuffer &commandBuffer)
{
	gli::texture2d tex2D(gli::load(fileName));

	assert(!tex2D.empty());

	uint32_t width = static_cast<uint32_t>(tex2D[0].extent().x);
	uint32_t height = static_cast<uint32_t>(tex2D[0].extent().y);
	uint32_t mipLevels = static_cast<uint32_t>(tex2D.levels());

	// Get device properites for the requested texture format
	VkFormatProperties formatProperties;
	vkGetPhysicalDeviceFormatProperties(device.getPhysicalDevice()->getHandle(), format, &formatProperties);

	// Only use linear tiling if forced
	if (forceLinearTiling)
	{
		// Don't use linear if format is not supported for (linear) shader sampling
		//useStaging = !(formatProperties.linearTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_BIT);
	}

	// Create a host-visible staging buffer that contains the raw image data

	VulkanBuffer stagingBuffer(device, static_cast<uint32_t>(tex2D.size()), VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
	VkDeviceMemory memory = device.allocateMemory(stagingBuffer.getMemoryTypeBits(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, tex2D.size());
	stagingBuffer.bindToMemory(memory, 0);
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
	//	Create the immage buffer
	VulkanImage *image = new VulkanImage(device.getHandle(), format, mipLevels, width, height);
	//	Grab memory requirements
	uint32_t memoryType = image->getMemoryRequirements().memoryTypeBits;
	uint32_t memorySize = static_cast<uint32_t>(image->getMemoryRequirements().size);
	//	Allocate memory
	VkDeviceMemory imageMemory = device.allocateMemory(memoryType, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, memorySize);
	//	Wrap the memory
	VulkanDeviceMemory imageMemoryWrapper(device.getHandle(), imageMemory);
	//	Bind the buffer to the memory
	image->bindToMemory(imageMemoryWrapper.getHandle(), 0);

	// Image barrier for optimal image

	VulkanImageSubResourceRange subresourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, mipLevels, 0, 1);

	VulkanImageMemoryBarrier imageMemoryBarrier(image->getHandle(), VK_IMAGE_ASPECT_COLOR_BIT, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, *(subresourceRange.getRaw()));

	commandBuffer.addBarriers(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, nullptr, 0, nullptr, 0, imageMemoryBarrier.getRaw(), 1);

	image->copyFromBuffer(commandBuffer.getHandle(), stagingBuffer.getHandle(), bufferCopyRegions);

	VulkanSampler sampler(device, static_cast<float>(mipLevels));

	VulkanImageSubResourceRange subResourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, mipLevels, 0, 1);

	VkImageSubresourceRange *rawSubResource = subResourceRange.getRaw();

	VulkanImageView imageview(device.getHandle(), VK_NULL_HANDLE, format, VK_IMAGE_VIEW_TYPE_2D, *rawSubResource);

	VulkanDescriptorImageInfo descriptorImageInfo(VK_IMAGE_LAYOUT_GENERAL, imageview.getHandle(), sampler.getHandle());
}