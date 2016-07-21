#include "VulkanPresentationSurface.h"

#include "VulkanDevice.h"

VulkanPresentationSurface::VulkanPresentationSurface(VulkanDevice &device, VkInstance instance, VkSurfaceKHR surface) : m_device(&device), m_instance(instance), m_surface(surface)
{
	auto gpu = m_device->getPhysicalDevice();

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(gpu, m_surface, &m_capabilities);
	if (m_capabilities.currentExtent.width < UINT32_MAX)
	{
		m_width = m_capabilities.currentExtent.width;

		m_height = m_capabilities.currentExtent.height;
	}

	// Count the number of available formats
	uint32_t format_count = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, m_surface, &format_count, nullptr);
	if (format_count == 0)
	{
		// Display error
		//assert(0 && "Surface formats missing.");
		// Exit if there are no available formats
		std::exit(-1);
	}

	// Enumerate the available formats
	std::vector<VkSurfaceFormatKHR> formats(format_count);
	vkGetPhysicalDeviceSurfaceFormatsKHR(gpu, m_surface, &format_count, formats.data());
	if (formats[0].format == VK_FORMAT_UNDEFINED)
	{
		m_surfaceFormat.format = VK_FORMAT_B8G8R8A8_UNORM;
		m_surfaceFormat.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	}
	else
	{
		m_surfaceFormat = formats[0];
	}
}

VulkanPresentationSurface::~VulkanPresentationSurface()
{
	vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
}

VkSurfaceKHR
VulkanPresentationSurface::getHandle()
{
	return m_surface;
}

VkSurfaceKHR*
VulkanPresentationSurface::getHandlePtr()
{
	return &m_surface;
}

VkSurfaceFormatKHR*
VulkanPresentationSurface::getFormat()
{
	return &m_surfaceFormat;
}

VkSurfaceCapabilitiesKHR*
VulkanPresentationSurface::getCapabilities()
{
	return &m_capabilities;
}

uint32_t
VulkanPresentationSurface::getWidth()
{
	return m_width;
}

uint32_t
VulkanPresentationSurface::getHeight()
{
	return m_height;
}