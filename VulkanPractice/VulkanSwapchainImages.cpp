#include "VulkanSwapchainImages.h"

#include "VulkanDevice.h"
#include "VulkanImageView.h"
#include "VulkanFrameBuffer.h"
#include "VulkanSwapChain.h"
#include "VulkanPresentationSurface.h"
#include "VulkanImageSubResourceRange.h"
#include "VulkanDeviceMemory.h"
#include "VulkanImage2D.h"

#include "VulkanCommandBuffer.h"

VulkanSwapchainImages::VulkanSwapchainImages(VulkanDevice &device, VulkanSwapchain &swapchain, VulkanPresentationSurface &surface, VkRenderPass renderPass) : m_device(&device)
{
	uint32_t imageCount = swapchain.getImageCount();
	m_images = new VkImage[imageCount];
	vkGetSwapchainImagesKHR(m_device->getHandle(), swapchain.getHandle(), &imageCount, m_images);
	
	VulkanImageSubResourceRange subResourceRange(VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
	VkImageSubresourceRange *rawResourceRange = subResourceRange.getRaw();

	VulkanImageSubResourceRange depthSubResourceRange(VK_IMAGE_ASPECT_DEPTH_BIT, 0, 1, 0, 1);
	VkImageSubresourceRange *depthRawResourceRange = subResourceRange.getRaw();

	VkFormat depthFormat = device.findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	m_depthImage = new VulkanImage2D(m_device->getHandle(), depthFormat, VK_IMAGE_LAYOUT_PREINITIALIZED, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 1, surface.getWidth(), surface.getHeight());

	VkImageMemoryBarrier setDepthLayoutBarrier = m_depthImage->createSetLayoutBarrier(VK_IMAGE_ASPECT_DEPTH_BIT, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, *(subResourceRange.getRaw()));

	VulkanCommandBuffer oneTimeBuffer(device.beginSingleTimeCommands());
	oneTimeBuffer.addBarriers(VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, nullptr, 0, nullptr, 0, &setDepthLayoutBarrier, 1);
	device.endSingleTimeCommands(oneTimeBuffer.getHandle());

	VkDeviceMemory depthMemory = device.allocateMemory(m_depthImage->getMemoryTypeBits(), VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthImage->getSize());
	m_depthImageMemory = new VulkanDeviceMemory(device.getHandle(), depthMemory);
	m_depthImage->bindToMemory(*m_depthImageMemory, 0);

	m_depthImageView = new VulkanImageView(m_device->getHandle(), m_depthImage->getHandle(), depthFormat, VK_IMAGE_VIEW_TYPE_2D, *depthRawResourceRange);

	for (uint32_t i = 0; i < imageCount; ++i)
	{
		// create imageViews
		VulkanImageView *imageView = new VulkanImageView(m_device->getHandle(), m_images[i], surface.getFormat()->format, VK_IMAGE_VIEW_TYPE_2D, *rawResourceRange);
		m_imageViews.push_back(imageView->getHandle());
		m_imageViewWrappers.push_back(imageView);

		// create framebuffers
		VulkanFrameBuffer *frameBuffer = new VulkanFrameBuffer(m_device->getHandle());
		frameBuffer->addAttachment(m_imageViews.at(i));
		frameBuffer->addAttachment(m_depthImageView->getHandle());
		frameBuffer->initialize(renderPass, surface.getWidth(), surface.getHeight());
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