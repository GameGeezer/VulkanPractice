#include "VulkanSwapchainImages.h"

#include "VulkanDevice.h"
#include "VulkanImageView.h"
#include "VulkanFrameBuffer.h"
#include "VulkanSwapChain.h"
#include "VulkanPresentationSurface.h"
#include "VulkanImageSubResourceRange.h"

VulkanSwapchainImages::VulkanSwapchainImages(VulkanDevice &device, VulkanSwapchain &swapchain, VulkanPresentationSurface &surface, VkRenderPass renderPass) : m_device(&device)
{
	uint32_t imageCount = swapchain.getImageCount();
	m_images = new VkImage[imageCount];
	vkGetSwapchainImagesKHR(m_device->getHandle(), swapchain.getHandle(), &imageCount, m_images);

	VulkanImageSubResourceRange subResourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
	VkImageSubresourceRange *rawResourceRange = subResourceRange.getRaw();

	for (uint32_t i = 0; i < imageCount; ++i)
	{
		// create imageViews
		VulkanImageView *imageView = new VulkanImageView(m_device->getHandle(), m_images[i], surface.getFormat()->format, VK_IMAGE_VIEW_TYPE_2D, *rawResourceRange);
		m_imageViews.push_back(imageView->getHandle());
		m_imageViewWrappers.push_back(imageView);

		// create framebuffers
		VulkanFrameBuffer *frameBuffer = new VulkanFrameBuffer(m_device->getHandle(), renderPass, m_imageViews.at(i), surface.getWidth(), surface.getHeight());
		m_frameBuffers.push_back(frameBuffer->getHandle());
		m_frameBufferWrappers.push_back(frameBuffer);
	}
}

VulkanSwapchainImages::~VulkanSwapchainImages()
{
	for (int i = 0; i < m_imageViews.size(); ++i)
	{
		delete m_imageViewWrappers.at(i);
	}

	for (int i = 0; i < m_frameBuffers.size(); ++i)
	{
		delete m_frameBufferWrappers.at(i);
	}
}

VkFramebuffer
VulkanSwapchainImages::getFrameBuffer(uint32_t index)
{
	return m_frameBuffers.at(index);
}