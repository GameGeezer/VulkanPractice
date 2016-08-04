#include "VulkanImageSubResourceRange.h"

VulkanImageSubResourceRange::VulkanImageSubResourceRange(VkImageAspectFlags aspectMask, uint32_t baseMipLevel, uint32_t levelCount, uint32_t baseArrayLayer, uint32_t layerCount)
{
	m_imageSubResourceRange.aspectMask = aspectMask;
	m_imageSubResourceRange.baseMipLevel = baseMipLevel;
	m_imageSubResourceRange.levelCount = levelCount;
	m_imageSubResourceRange.baseArrayLayer = baseArrayLayer;
	m_imageSubResourceRange.layerCount = layerCount;
}

VulkanImageSubResourceRange::~VulkanImageSubResourceRange()
{

}

VkImageSubresourceRange*
VulkanImageSubResourceRange::getRaw()
{
	return &m_imageSubResourceRange;
}