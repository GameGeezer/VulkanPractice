#pragma once

#include <vulkan\vulkan.h>

class VulkanDevice;

class VulkanPresentationSurface
{
public:
	VulkanPresentationSurface(VulkanDevice &device, VkInstance instance, VkSurfaceKHR surface);

	~VulkanPresentationSurface();

	VkSurfaceKHR
	getHandle();

	VkSurfaceKHR*
	getHandlePtr();

	VkSurfaceFormatKHR*
	getFormat();

	VkSurfaceCapabilitiesKHR*
	getCapabilities();

	uint32_t
	getWidth();

	uint32_t
	getHeight();

private:
	VulkanDevice			*	m_device;
	VkInstance					m_instance;
	VkSurfaceKHR				m_surface;
	VkSurfaceFormatKHR			m_surfaceFormat = {};
	VkSurfaceCapabilitiesKHR	m_capabilities = {};

	uint32_t					m_width, m_height;
};