#include "VulkanSwapchainImages.h"

#include "VulkanDevice.h"
#include "VulkanImageView.h"
#include "FrameBuffer.h"
#include "VulkanSwapChain.h"
#include "VulkanPresentationSurface.h"

VulkanSwapchainImages::VulkanSwapchainImages(VulkanDevice &device, VulkanSwapchain &swapchain, VulkanPresentationSurface &surface, VkRenderPass renderPass) : m_device(&device)
{
	uint32_t imageCount = swapchain.getImageCount();
	m_images = new VkImage[imageCount];
	vkGetSwapchainImagesKHR(m_device->getDevice(), swapchain.getHandle(), &imageCount, m_images);

	for (uint32_t i = 0; i < imageCount; ++i)
	{
		// create imageViews
		VulkanImageView *imageView = new VulkanImageView(m_device->getDevice(), m_images[i], surface.getFormat()->format);
		m_imageViews.push_back(imageView->getHandle());
		m_imageViewWrappers.push_back(imageView);

		// create framebuffers
		FrameBuffer *frameBuffer = new FrameBuffer(m_device->getDevice(), renderPass, m_imageViews.at(i), surface.getWidth(), surface.getHeight());
		m_frameBuffers.push_back(frameBuffer->getHandle());
		m_frameBufferWrapperss.push_back(frameBuffer);
	}
}

VulkanSwapchainImages::~VulkanSwapchainImages()
{
	for (int i = 0; i < m_imageViews.size(); ++i)
	{
		delete m_imageViews.at(i);
	}

	for (int i = 0; i < m_frameBuffers.size(); ++i)
	{
		delete m_frameBuffers.at(i);
	}
}

VkFramebuffer
VulkanSwapchainImages::getFrameBuffer(uint32_t index)
{
	return m_frameBuffers.at(index);
}