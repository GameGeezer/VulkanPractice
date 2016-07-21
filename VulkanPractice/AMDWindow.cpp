#include "AMDWindow.h"

#include "Shared.h"

#include "VulkanDevice.h"
#include "VulkanPresentationSurface.h"
#include "VulkanSwapChain.h"
#include "VulkanSwapchainImages.h"

#include "RenderPass.h"
#include "RenderSubPass.h"
#include"AttachmentDescription.h"

namespace AMD
{
		///////////////////////////////////////////////////////////////////////////////
	IWindow::~IWindow()
	{
		
	}

	RenderPass* CreateRenderPass(VkDevice device, VkFormat swapchainFormat)
	{
		AttachmentDescription attachmentDescription(VK_SAMPLE_COUNT_1_BIT, swapchainFormat, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, false);

		VkAttachmentReference attachmentReference = {};
		attachmentReference.attachment = 0;
		attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		RenderSubPass subpass;
		subpass.addColorAttachment(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
		subpass.initialize();

		RenderPass *renderPass = new RenderPass(device);
		renderPass->addSubPass(subpass);
		renderPass->addAttachmentDescription(attachmentDescription);
		renderPass->initialize();

		return renderPass;
	}

	///////////////////////////////////////////////////////////////////////////////
	Window::Window(VkInstance instance, VulkanDevice& device, char *title, const uint32_t width, const uint32_t height, uint32_t swapchainImageCount): m_vulkanInstance(instance), m_device(&device), m_title(title), m_width(width), m_height(height), m_swapchainImageCount(swapchainImageCount)
	{
		initOsWindow();
		m_surface = new VulkanPresentationSurface(*m_device, m_vulkanInstance, initOSSurface());
		m_swapchain = new VulkanSwapchain(*m_device, *m_surface, m_swapchainImageCount);
		m_renderPass = CreateRenderPass(m_device->getDevice(), m_surface->getFormat()->format);

		m_swapchainImages = new VulkanSwapchainImages(*m_device, *m_swapchain, *m_surface, m_renderPass->getHandle());
	//	initSwapchainImages();
	}

	Window::~Window()
	{
	//	deInitSwapchainImages();
		delete m_swapchain;
		delete m_surface;
		deInitOSWindow();
	}

	///////////////////////////////////////////////////////////////////////////////
	bool IWindow::IsClosed() const
	{
		return IsClosedImpl();
	}

	void
	Window::beginRenderPass(VkCommandBuffer commandBuffer, uint32_t index)
	{
		VkClearValue clearValue = {};

		clearValue.color.float32[0] = 0.042f;
		clearValue.color.float32[1] = 0.042f;
		clearValue.color.float32[2] = 0.042f;
		clearValue.color.float32[3] = 1.0f;

		m_renderPass->begin(commandBuffer, m_swapchainImages->getFrameBuffer(index), m_width, m_height, clearValue, 1);
	}

	void
	Window::endRenderPass(VkCommandBuffer commandBuffer)
	{
		m_renderPass->end(commandBuffer);
	}

	VkSurfaceKHR
	Window::getSurface()
	{
		return m_surface->getHandle();
	}

	VkSwapchainKHR
	Window::getSwapchain()
	{
		return m_swapchain->getHandle();
	}


	///////////////////////////////////////////////////////////////////////////////
	uint32_t
	Window::getWidth()
	{
		return m_width;
	}

	///////////////////////////////////////////////////////////////////////////////
	uint32_t
	Window::getHeight()
	{
		return m_height;
	}

	VkFormat
	Window::getSurfaceFormat()
	{
		return m_surface->getFormat()->format;
	}


	RenderPass*
	Window::getRenderPass()
	{
		return m_renderPass;
	}

}   // namespace AMD