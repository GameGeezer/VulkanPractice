#include "VulkanImage.h"

#include "Shared.h"

VulkanImage::VulkanImage(VkDevice device, VkFormat format, uint32_t mipMapLevels, uint32_t width, uint32_t height) : m_device(device)
{
	// Create optimal tiled target image
	VkImageCreateInfo imageCreateInfo;
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	imageCreateInfo.format = format;
	imageCreateInfo.mipLevels = mipMapLevels;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	// Set initial layout of the image to undefined
	imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageCreateInfo.extent = { width, height, 1 };
	imageCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

	ErrorCheck(vkCreateImage(m_device, &imageCreateInfo, nullptr, &m_image));

	vkGetImageMemoryRequirements(m_device, m_image, &m_memoryRequirements);
}

VulkanImage::~VulkanImage()
{
	vkDestroyImage(m_device, m_image, nullptr);
}

void
VulkanImage::bindToMemory(VkDeviceMemory deviceMemory, uint32_t memoryOffset)
{
	ErrorCheck(vkBindImageMemory(m_device, m_image, deviceMemory, memoryOffset));
}

void
VulkanImage::copyFromBuffer(VkCommandBuffer commandBuffer, VkBuffer stagingBuffer, std::vector<VkBufferImageCopy> &bufferCopyRegions)
{
	vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(bufferCopyRegions.size()), bufferCopyRegions.data());
}

VkImage
VulkanImage::getHandle()
{
	return m_image;
}

VkMemoryRequirements
VulkanImage::getMemoryRequirements()
{
	return m_memoryRequirements;
}