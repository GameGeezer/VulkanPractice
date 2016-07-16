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
	void setupLayersAndExtensions();

	void initRenderPass();
	void initDescriptorSetLayout();

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

	VkDescriptorSetLayout					m_descriptorSetLayout = VK_NULL_HANDLE;
	VkPipelineLayout						m_pipelineLayout = VK_NULL_HANDLE;
	VkRenderPass							m_renderPass = VK_NULL_HANDLE;

	uint32_t								m_graphicsFamilyIndex = 0;

	VkFormat								depthFormat;

	std::vector<const char*>				instanceLayers;
	std::vector<const char*>				instanceExtensions;
	std::vector<const char*>				deviceExtensions;

	VkDebugReportCallbackEXT				_debug_report = VK_NULL_HANDLE;
	VkDebugReportCallbackCreateInfoEXT		_debug_callback_create_info = {};
};