#include "Window.h"
#include "Renderer.h"
#include "Shared.h"

#include <assert.h>

Window::Window(Renderer * renderer, uint32_t size_x, uint32_t size_y, std::string name)
{
	m_renderer = renderer;
	m_width = size_x;
	m_height = size_y;
	m_name = name;

	initOSWindow();
	initSurface();
	initSwapchain();
	initSwapchainImages();
}

Window::~Window()
{
	deInitSwapchainImages();
	deInitSwapchain();
	deInitSurface();
	deInitOSWindow();
}


void Window::requestClose()
{
	m_isAlive = false;
}

bool Window::isAlive()
{
	return m_isAlive;
}

void Window::update()
{
	updateOSWindow();
}

void Window::initSurface()
{
	initOSSurface();

	auto gpu = m_renderer->GetVulkanPhysicalDevice();
	VkBool32 WSI_supported = false;
	vkGetPhysicalDeviceSurfaceSupportKHR(gpu, m_renderer->GetVulkanGraphicsQueueFamilyIndex(), m_surface, &WSI_supported);
	if (!WSI_supported)
	{
		// Display error
		assert(0 && "WSI not supported");
		// Exit if surfaces are not supported
		std::exit(-1);
	}

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, m_surface, &_surface_capabilities);
	if (_surface_capabilities.currentExtent.width < UINT32_MAX)
	{
		m_width = _surface_capabilities.currentExtent.width;

		m_height = _surface_capabilities.currentExtent.height;
	}

	// Count the number of available formats
	uint32_t format_count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, m_surface, &format_count, nullptr);
	if (format_count == 0)
	{
		// Display error
		assert(0 && "Surface formats missing.");
		// Exit if there are no available formats
		std::exit(-1);
	}

	// Enumerate the available formats
	std::vector<VkSurfaceFormatKHR> formats(format_count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, m_surface, &format_count, formats.data());
	if (formats[0].format == VK_FORMAT_UNDEFINED)
	{
		_surface_format.format = VK_FORMAT_B8G8R8A8_UNORM;
		_surface_format.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	}
	else
	{
		_surface_format = formats[0];
	}
}

void Window::deInitSurface()
{
	vkDestroySurfaceKHR(m_renderer->GetVulkanInstance(), m_surface, nullptr);
}

void Window::initSwapchain()
{

	// The code above will work just fine in our tutorials and likely on every possible implementation of vulkan as well
	// so this change isn't that important. Just to be absolutely sure we don't go over or below the given limits we should check this a
	// little bit different though. maxImageCount can actually be zero in which case the amount of swapchain images do not have an
	// upper limit other than available memory. It's also possible that the swapchain image amount is locked to a certain
	// value on certain systems. The code below takes into consideration both of these possibilities.
	if (m_swapchainImageCount < _surface_capabilities.minImageCount + 1)
	{
		m_swapchainImageCount = _surface_capabilities.minImageCount + 1;
	}

	if (_surface_capabilities.maxImageCount > 0 && m_swapchainImageCount > _surface_capabilities.maxImageCount) {
		m_swapchainImageCount = _surface_capabilities.maxImageCount;
	}

	VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
	{
		uint32_t present_mode_count = 0;
		ErrorCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(m_renderer->GetVulkanPhysicalDevice(), m_surface, &present_mode_count, nullptr));
		std::vector<VkPresentModeKHR> present_mode_list(present_mode_count);
		ErrorCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(m_renderer->GetVulkanPhysicalDevice(), m_surface, &present_mode_count, present_mode_list.data()));
		for (auto m : present_mode_list) {
			if (m == VK_PRESENT_MODE_MAILBOX_KHR) present_mode = m;
		}
	}

	VkSwapchainCreateInfoKHR swapchain_create_info{};
	swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_create_info.surface = m_surface;
	swapchain_create_info.minImageCount = m_swapchainImageCount;
	swapchain_create_info.imageFormat = _surface_format.format;
	swapchain_create_info.imageColorSpace = _surface_format.colorSpace;
	swapchain_create_info.imageExtent.width = m_width;
	swapchain_create_info.imageExtent.height = m_height;
	swapchain_create_info.imageArrayLayers = 1;
	swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchain_create_info.queueFamilyIndexCount = 0;
	swapchain_create_info.pQueueFamilyIndices = nullptr;
	swapchain_create_info.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_create_info.presentMode = present_mode;
	swapchain_create_info.clipped = VK_TRUE;
	swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

	ErrorCheck(vkCreateSwapchainKHR(m_renderer->GetVulkanDevice(), &swapchain_create_info, nullptr, &m_swapchain));

	ErrorCheck(vkGetSwapchainImagesKHR(m_renderer->GetVulkanDevice(), m_swapchain, &m_swapchainImageCount, nullptr));
}

void Window::deInitSwapchain()
{
	vkDestroySwapchainKHR(m_renderer->GetVulkanDevice(), m_swapchain, nullptr);
}

void Window::initSwapchainImages()
{
	m_swapchainImages.resize(m_swapchainImageCount);
	m_swapchainImageViews.resize(m_swapchainImageCount);

	ErrorCheck(vkGetSwapchainImagesKHR(m_renderer->GetVulkanDevice(), m_swapchain, &m_swapchainImageCount, m_swapchainImages.data()));

	for (uint32_t i = 0; i < m_swapchainImageCount; ++i) {
		VkImageViewCreateInfo image_view_create_info{};
		image_view_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		image_view_create_info.image = m_swapchainImages[i];
		image_view_create_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		image_view_create_info.format = _surface_format.format;
		image_view_create_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		image_view_create_info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_view_create_info.subresourceRange.baseMipLevel = 0;
		image_view_create_info.subresourceRange.levelCount = 1;
		image_view_create_info.subresourceRange.baseArrayLayer = 0;
		image_view_create_info.subresourceRange.layerCount = 1;

		ErrorCheck(vkCreateImageView(m_renderer->GetVulkanDevice(), &image_view_create_info, nullptr, &m_swapchainImageViews[i]));
	}
}

void Window::deInitSwapchainImages()
{
	for (auto view : m_swapchainImageViews) {
		vkDestroyImageView(m_renderer->GetVulkanDevice(), view, nullptr);
	}
}