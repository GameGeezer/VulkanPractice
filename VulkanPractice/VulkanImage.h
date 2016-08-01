#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanImage
{
public:
	VulkanImage(VkDevice device, VkFormat format, uint32_t mipMapLevels, uint32_t width, uint32_t height);

	~VulkanImage();

	void
	bindToMemory(VkDeviceMemory deviceMemory, uint32_t memoryOffset);

	void
	copyFromBuffer(VkCommandBuffer commandBuffer, VkBuffer stagingBuffer, std::vector<VkBufferImageCopy> &bufferCopyRegions);

	VkImage
	getHandle();

	VkMemoryRequirements
	getMemoryRequirements();

private:
	VkDevice				m_device;
	VkImage					m_image;
	VkMemoryRequirements	m_memoryRequirements;
};