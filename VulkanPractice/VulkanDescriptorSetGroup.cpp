#include "VulkanDescriptorSetGroup.h"

VulkanDescriptorSetGroup::VulkanDescriptorSetGroup(VkDevice device) : m_device(device)
{

}

VulkanDescriptorSetGroup::~VulkanDescriptorSetGroup()
{

}

void
VulkanDescriptorSetGroup::initialize(VkDescriptorPool pool, uint32_t numberToAllocate)
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

	vkAllocateDescriptorSets(m_device, &allocInfo, m_descriptorSets);

	m_initailized = true;
}

void
VulkanDescriptorSetGroup::addLayout(VkDescriptorSetLayout layout)
{
	m_layouts.push_back(layout);
}

uint32_t
VulkanDescriptorSetGroup::getCount()
{
	return m_count;
}

VkDescriptorSet
VulkanDescriptorSetGroup::getDescriptorSetAtIndex(uint32_t index)
{
	return m_descriptorSets[index];
}