#include "VulkanDescriptorSetLayout.h"

#include "Shared.h"

VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VkDevice device) : m_device(device)
{



}

VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
{
	vkDestroyDescriptorSetLayout(m_device, m_layout, nullptr);
}

VkDescriptorSetLayout
VulkanDescriptorSetLayout::getHandle()
{
	return m_layout;
}

void
VulkanDescriptorSetLayout::initialize()
{
	if (m_initialized)
	{
		throw "VulkanDescriptorSetLayout has already been initialized";
	}

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = static_cast<uint32_t>(m_bindings.size());
	layoutInfo.pBindings = m_bindings.data();

	ErrorCheck(vkCreateDescriptorSetLayout(m_device, &layoutInfo, nullptr, &m_layout));

	m_initialized = true;
}

void
VulkanDescriptorSetLayout::addBinding(uint32_t binding, uint32_t count, VkShaderStageFlags stageflags)
{
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBinding.binding = binding;
	layoutBinding.descriptorCount = count;
	layoutBinding.stageFlags = stageflags;
	layoutBinding.pImmutableSamplers = nullptr;

	m_bindings.push_back(layoutBinding);
}