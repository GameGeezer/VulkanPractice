#include "VulkanSwapChain.h"

#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"

#include "VulkanPresentationSurface.h"


struct SwapchainFormatColorSpace
{
	VkFormat format;
	VkColorSpaceKHR colorSpace;
};

///////////////////////////////////////////////////////////////////////////////
SwapchainFormatColorSpace GetSwapchainFormatAndColorspace(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	uint32_t surfaceFormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr);

	std::vector<VkSurfaceFormatKHR> surfaceFormats{ surfaceFormatCount };
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data());

	SwapchainFormatColorSpace result;

	if (surfaceFormatCount == 1 && surfaceFormats.front().format == VK_FORMAT_UNDEFINED)
	{
		result.format = VK_FORMAT_R8G8B8A8_UNORM;
	}
	else
	{
		result.format = surfaceFormats.front().format;
	}

	result.colorSpace = surfaceFormats.front().colorSpace;

	return result;
}

VulkanSwapchain::VulkanSwapchain(VulkanDevice &device, VulkanPresentationSurface &surface, uint32_t imageCount) : m_device(&device), m_surface(&surface), m_imageCount(imageCount)
{
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(m_device->getPhysicalDevice()->getHandle(), m_surface->getHandle(), &presentModeCount, nullptr);

	std::vector<VkPresentModeKHR> presentModes{ presentModeCount };

	vkGetPhysicalDeviceSurfacePresentModesKHR(m_device->getPhysicalDevice()->getHandle(), m_surface->getHandle(), &presentModeCount, presentModes.data());

	VkSurfaceCapabilitiesKHR *surfaceCapibilities = m_surface->getCapabilities();
	VkExtent2D swapChainSize = {};
	swapChainSize = surfaceCapibilities->currentExtent;
	//assert(static_cast<int> (swapChainSize.width) == surfaceWidth);
	//assert(static_cast<int> (swapChainSize.height) == surfaceHeight);

	//	uint32_t swapChainImageCount = backbufferCount;
	//	assert(swapChainImageCount >= surfaceCapabilities.minImageCount);

	// 0 indicates unlimited number of images
	//	if (surfaceCapabilities.maxImageCount != 0)
	{
		//	assert(swapChainImageCount < surfaceCapabilities.maxImageCount);
	}

	VkSurfaceTransformFlagBitsKHR surfaceTransformFlags;

	if (surfaceCapibilities->supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
	{
		surfaceTransformFlags = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}
	else
	{
		surfaceTransformFlags = surfaceCapibilities->currentTransform;
	}

	auto swapchainFormatColorSpace = GetSwapchainFormatAndColorspace(m_device->getPhysicalDevice()->getHandle(), m_surface->getHandle());

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = m_surface->getHandle();
	swapchainCreateInfo.minImageCount = m_imageCount;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.preTransform = surfaceTransformFlags;
	swapchainCreateInfo.imageColorSpace = swapchainFormatColorSpace.colorSpace;
	swapchainCreateInfo.imageFormat = swapchainFormatColorSpace.format;
	swapchainCreateInfo.pQueueFamilyIndices = nullptr;
	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.clipped = VK_TRUE;
	swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.imageExtent = swapChainSize;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

	vkCreateSwapchainKHR(m_device->getDevice(), &swapchainCreateInfo, nullptr, &m_swapchain);

	// IMAGE VIEWS


}

VulkanSwapchain::~VulkanSwapchain()
{
	vkDestroySwapchainKHR(m_device->getDevice(), m_swapchain, nullptr);
}

VkSwapchainKHR
VulkanSwapchain::getHandle()
{
	return m_swapchain;
}

uint32_t
VulkanSwapchain::getImageCount()
{
	return m_imageCount;
}