#include "VulkanDescriptorSet.h"

VulkanDescriptorSet::VulkanDescriptorSet(VkDevice device) : m_device(device)
{

}

VulkanDescriptorSet::~VulkanDescriptorSet()
{

}

void
VulkanDescriptorSet::initialize(VkDescriptorPool pool, uint32_t numberToAllocate)
{
	if (m_initailized)
	{
		throw "VulkanDescriptorSetGroup has already been initialized";
	}

	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = pool;
	allocInfo.descriptorSetCount = numberToAllocate;
	allocInfo.pSetLayouts = m_layouts.data();

	vkAllocateDescriptorSets(m_device, &allocInfo, &m_descriptorSet);

	m_initailized = true;
}

void
VulkanDescriptorSet::addLayout(VkDescriptorSetLayout layout)
{
	m_layouts.push_back(layout);
}

uint32_t
VulkanDescriptorSet::getCount()
{
	return m_count;
}

VkDescriptorSet
VulkanDescriptorSet::getHandle()
{
	return m_descriptorSet;
}