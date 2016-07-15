#pragma once

#include "Platform.h"

#include <vector>
#include <string>

class Renderer
{
public:
	Renderer();
	~Renderer();

	const VkInstance						GetVulkanInstance()	const;
	const VkPhysicalDevice					GetVulkanPhysicalDevice() const;
	const VkDevice							GetVulkanDevice() const;
	const VkQueue							GetVulkanQueue() const;
	const uint32_t							GetVulkanGraphicsQueueFamilyIndex() const;
	const VkPhysicalDeviceProperties	&	GetVulkanPhysicalDeviceProperties() const;

private:
	void _SetupLayersAndExtensions();

	void initInstance();
	void deInitInstance();

	void initDevice();
	void deInitDevice();

	void setupDebug();
	void initDebug();
	void deInitDebug();

	VkInstance								m_instance = VK_NULL_HANDLE;
	VkPhysicalDevice						m_gpu = VK_NULL_HANDLE;
	VkDevice								m_device = VK_NULL_HANDLE;
	VkQueue									m_queue = VK_NULL_HANDLE;
	VkPhysicalDeviceProperties				m_gpuProperties = {};

	uint32_t								m_graphicsFamilyIndex = 0;

	std::vector<const char*>				_instance_layers;
	std::vector<const char*>				_instance_extensions;
	//	std::vector<const char*>				_device_layers;					// depricated
	std::vector<const char*>				_device_extensions;

	VkDebugReportCallbackEXT				_debug_report = VK_NULL_HANDLE;
	VkDebugReportCallbackCreateInfoEXT		_debug_callback_create_info = {};
};