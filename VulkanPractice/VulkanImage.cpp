#include "VulkanImage.h"

#include "Shared.h"

#include "VulkanImageMemoryBarrier.h"
#include "VulkanImageSubResourceRange.h"
#include "VulkanDeviceMemory.h"


VulkanImage::VulkanImage(
		VkDevice device, 
		VkImageType imageType, 
		VkFormat format, 
		VkImageLayout initialLayout,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		uint32_t mipMapLevels, 
		uint32_t width, 
		uint32_t height,
		uint32_t depth): m_device(device), m_currentLayout(initialLayout), m_mipMapLevels(mipMapLevels)
{
	VkImageCreateInfo imageCreateInfo = {};
	imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageCreateInfo.imageType = imageType;
	imageCreateInfo.extent.width = width;
	imageCreateInfo.extent.height = height;
	imageCreateInfo.extent.depth = depth;
	imageCreateInfo.mipLevels = mipMapLevels;
	imageCreateInfo.arrayLayers = 1;
	imageCreateInfo.format = format;
	imageCreateInfo.tiling = tiling;
	imageCreateInfo.initialLayout = initialLayout;
	imageCreateInfo.usage = usage;
	imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	ErrorCheck(vkCreateImage(m_device, &imageCreateInfo, nullptr, &m_image));

	vkGetImageMemoryRequirements(m_device, m_image, &m_memoryRequirements);
}

VulkanImage::~VulkanImage()
{
	vkDestroyImage(m_device, m_image, nullptr);
}

void
VulkanImage::getSubresourceLayout(VkImageSubresource &subResources, VkSubresourceLayout &out_subresourceLayout)
{
	vkGetImageSubresourceLayout(m_device, m_image, &subResources, &out_subresourceLayout);
}

void
VulkanImage::bindToMemory(VulkanDeviceMemory &deviceMemory, uint32_t memoryOffset)
{
	ErrorCheck(vkBindImageMemory(m_device, m_image, deviceMemory.getHandle(), memoryOffset));

	m_memoryBoundTo = &deviceMemory;
}

void
VulkanImage::copyFromBuffer(VkCommandBuffer commandBuffer, VkBuffer stagingBuffer, std::vector<VkBufferImageCopy> &bufferCopyRegions)
{
	vkCmdCopyBufferToImage(commandBuffer, stagingBuffer, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, static_cast<uint32_t>(bufferCopyRegions.size()), bufferCopyRegions.data());
}

VkImageMemoryBarrier
VulkanImage::createSetLayoutBarrier(VkImageAspectFlags aspectFlags, VkImageLayout newLayout, VkImageSubresourceRange &subresourceRange)
{
	VulkanImageSubResourceRange subResourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);

	VulkanImageMemoryBarrier imageMemoryBarrier(m_image, aspectFlags, m_currentLayout, newLayout, subresourceRange);

	m_currentLayout = newLayout;

	return *(imageMemoryBarrier.getRaw());
}

void
VulkanImage::setMemory(void *data, uint32_t offset, size_t size)
{
	// Possible bug with offsets
	void *pData = nullptr;
	void *mapping = m_memoryBoundTo->map(0, getSize());
	memcpy(pData, data, size);
	//m_memoryBoundTo->copyFrom(mapping, data, offset, size);
	m_memoryBoundTo->unmap();
}

VkImage
VulkanImage::getHandle()
{
	return m_image;
}

// returns - Each bit itentifies a differnt memory type that is supported
uint32_t
VulkanImage::getMemoryTypeBits()
{
	return m_memoryRequirements.memoryTypeBits;
}

//	returns - Memory size ine bytes
VkDeviceSize
VulkanImage::getSize()
{
	return m_memoryRequirements.size;
}

//	returns -The alignment of the offset within the allocation in bytes
VkDeviceSize
VulkanImage::getAlignment()
{
	return m_memoryRequirements.alignment;
}

uint32_t
VulkanImage::getMipMapLevels()
{
	return m_mipMapLevels;
}