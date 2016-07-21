#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanDevice;
class VulkanImageView;
class FrameBuffer;
class VulkanSwapchain;
class VulkanPresentationSurface;

class VulkanSwapchainImages
{
public:
	VulkanSwapchainImages(VulkanDevice &device, VulkanSwapchain &swapchain, VulkanPresentationSurface &surface, VkRenderPass renderPass);

	~VulkanSwapchainImages();

	VkFramebuffer
	getFrameBuffer(uint32_t index);

private:
	VulkanDevice					*	m_device;

	VkImage							*	m_images;

	std::vector<VulkanImageView*>		m_imageViewWrappers;
	std::vector<FrameBuffer*>			m_frameBufferWrapperss;

	std::vector<VkImageView>			m_imageViews;
	std::vector<VkFramebuffer>			m_frameBuffers;
};