#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanInstance
{
public:
	VulkanInstance();

	~VulkanInstance();

	bool
	initialize();

	void
	addExtension(const char* extension);

	void
	addLayer(const char* layer);

	VkInstance
	getHandle();

private:
	VkInstance					m_instance;
	std::vector<const char*>	m_extensions;
	std::vector<const char*>	m_layers;

	bool						m_initialzed = false;
};