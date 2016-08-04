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
	//	binding		- Binding number of the entry which corresponds to the resource with the same binding number (in the shader stages)
	//	count		- The number of descriptors contained within the binding
	//	type		- 
	//	stageflags	- Which shader stages can access the resources for the binding
	void
	addBinding(uint32_t binding, uint32_t count, VkDescriptorType type, VkShaderStageFlags stageflags);

	VkDescriptorSetLayout
	getHandle();

private:
	VkDevice									m_device;
	VkDescriptorSetLayout						m_layout;
	std::vector<VkDescriptorSetLayoutBinding>	m_bindings;
	bool										m_initialized = false;
};