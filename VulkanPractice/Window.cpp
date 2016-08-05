#include "Window.h"

#include "Shared.h"

#include "VulkanDevice.h"
#include "VulkanPresentationSurface.h"
#include "VulkanSwapChain.h"
#include "VulkanSwapchainImages.h"

#include "VulkanRenderPass.h"
#include "VulkanRenderSubPass.h"
#include "VulkanAttachmentDescription.h"

#include <array>

KeyboardCallback* Window::KEYBOARD = new KeyboardCallback();

MouseClickCallback* Window::MOUSE_CLICK = new MouseClickCallback();

MouseMoveCallback* Window::MOUSE_MOVE = new MouseMoveCallback();

VulkanRenderPass* CreateRenderPass(VulkanDevice &device, VkFormat swapchainFormat)
{
	VulkanAttachmentDescription diffuseAttachmentDescription(VK_SAMPLE_COUNT_1_BIT, swapchainFormat, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, false);

	VkFormat depthFormat = device.findSupportedFormat({ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT }, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
	VulkanAttachmentDescription depthAttachmentDescription(VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, false);

	VulkanRenderSubPass subpass;
	subpass.addColorAttachment(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
	subpass.setDepthStencilAttachment(1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
	subpass.initialize();

	VulkanRenderPass *renderPass = new VulkanRenderPass(device.getHandle());
	renderPass->addSubPass(subpass);
	renderPass->addAttachmentDescription(*(diffuseAttachmentDescription.getDescription()));
	renderPass->addAttachmentDescription(*(depthAttachmentDescription.getDescription()));
	renderPass->initialize();

	return renderPass;
}

void
Window::update()
{
	updateOSWindow();
}

///////////////////////////////////////////////////////////////////////////////
Window::Window(VkInstance instance, VulkanDevice& device, char *title, const uint32_t width, const uint32_t height, uint32_t swapchainImageCount) : m_vulkanInstance(instance), m_device(&device), m_title(title), m_width(width), m_height(height), m_swapchainImageCount(swapchainImageCount)
{
	InitPlatform();
	initOsWindow();
	m_surface = new VulkanPresentationSurface(*m_device, m_vulkanInstance, initOSSurface());
	m_swapchain = new VulkanSwapchain(*m_device, *m_surface, m_swapchainImageCount);
	m_renderPass = CreateRenderPass(*m_device, m_surface->getFormat()->format);

	m_swapchainImages = new VulkanSwapchainImages(*m_device, *m_swapchain, *m_surface, m_renderPass->getHandle());
}

Window::~Window()
{
	delete m_renderPass;
	delete m_swapchainImages;
	delete m_swapchain;
	delete m_surface;
	deInitOSWindow();
}

uint32_t
Window::acquireNextImage(uint64_t timeout, VkSemaphore imageAcquiredSemaphore)
{
	uint32_t imageIndex;
	vkAcquireNextImageKHR(m_device->getHandle(), m_swapchain->getHandle(), timeout, imageAcquiredSemaphore, VK_NULL_HANDLE, &imageIndex);

	return imageIndex;
}

void
Window::beginRenderPass(VkCommandBuffer commandBuffer, uint32_t index)
{

	std::array<VkClearValue, 2> clearValues = {};
	clearValues[0].color = { 0.042f, 0.042f, 0.042f, 1.0f };
	clearValues[1].depthStencil = { 1.0f, 0 };

	m_renderPass->begin(commandBuffer, m_swapchainImages->getFrameBuffer(index), m_width, m_height, clearValues.data(), static_cast<uint32_t>(clearValues.size()));
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


VulkanRenderPass*
Window::getRenderPass()
{
	return m_renderPass;
}

bool
Window::isAlive()
{
	return m_isAlive;
}

void
Window::requestClose()
{
	m_isAlive = false;
}