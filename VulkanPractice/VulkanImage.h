#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanDeviceMemory;

class VulkanImage
{
public:
	
	//	device - Used in creation and destruction of the Image
	//	imageType - The dimensionality of the image
	//	format - The format and types of elements that will be contained within the image
	//	initialLayout - The inititial layout of all image sub resources (Must be VK_IMAGE_LAYOUT_UNDEFINED or VK_IMAGE_LAYOUT_PREINITIALIZED)
	//					~VK_IMAGE_LAYOUT_UNDEFINED - Supports no devices, contents are not preserved
	//					~VK_IMAGE_LAYOUT_PREINITIALIZED - Supports no devices, contents are preserved
	//	tiling - 
	//	usage - 
	//	mipMapLevels - the number of levels of detail available
	//	width - image width	(1D, 2D and 3D)
	//	height - image height (2D and 3D)
	//	depth - image depth (3D)
	VulkanImage(VkDevice device,
		VkImageType imageType,
		VkFormat format,
		VkImageLayout initialLayout,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		uint32_t mipMapLevels,
		uint32_t width,
		uint32_t height,
		uint32_t depth);

	~VulkanImage();

	void
	getSubresourceLayout(VkImageSubresource &subResources, VkSubresourceLayout &out_subresourceLayout);

	void
	bindToMemory(VulkanDeviceMemory &deviceMemory, uint32_t memoryOffset);

	void
	copyFromBuffer(VkCommandBuffer commandBuffer, VkBuffer stagingBuffer, std::vector<VkBufferImageCopy> &bufferCopyRegions);

	VkImageMemoryBarrier
	createSetLayoutBarrier(VkImageAspectFlags aspectFlags, VkImageLayout newLayout, VkImageSubresourceRange &subresourceRange);

	void
	setMemory(void *data, uint32_t offset, size_t size);

	VkImage
	getHandle();

	// returns - Each bit identifies a differnt memory type that is supported
	uint32_t
	getMemoryTypeBits();

	//	returns - Memory size ine bytes
	VkDeviceSize
	getSize();

	//	returns -The alignment of the offset within the allocation in bytes
	VkDeviceSize
	getAlignment();

	uint32_t
	getMipMapLevels();

private:
	VkDevice					m_device;
	VkImage						m_image;
	VkMemoryRequirements		m_memoryRequirements;

	VkImageLayout				m_currentLayout;

	uint32_t					m_mipMapLevels;

	VulkanDeviceMemory		*	m_memoryBoundTo;
};