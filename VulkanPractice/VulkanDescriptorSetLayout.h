#pragma once

#include <vulkan\vulkan.h>

#include <vector>

// A wrapper class for the creation of VkDescriptorSetLayouts.
class VulkanDescriptorSetLayout
{
public:

	VulkanDescriptorSetLayout(VkDevice device);

	~VulkanDescriptorSetLayout();

	void
	initialize();

	//
	//	binding		- binding number of the entry which corresponds to the resource with the same binding number (in the shader stages)
	//	count		- the number of descriptors contained within the binding
	//	stageflags	- Which shader stages can access the resources for the binding
	void
	addBinding(uint32_t binding, uint32_t count, VkShaderStageFlags stageflags);

	VkDescriptorSetLayout
	getHandle();

private:
	VkDevice									m_device;
	VkDescriptorSetLayout						m_layout;
	std::vector<VkDescriptorSetLayoutBinding>	m_bindings;
	bool										m_initialized = false;
};