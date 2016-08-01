#pragma once

#include <vulkan\vulkan.h>

class VulkanDeviceMemory
{
public:
	VulkanDeviceMemory(VkDevice device, VkDeviceMemory deviceMemory);

	~VulkanDeviceMemory();

	//	offset - The offset into the memory at which the mapping should start
	//	size - The size in bytes of memory that should be mapped
	//	return - A pointer to the mapping
	//	post - The designated memory is mapped
	void*
	map(VkDeviceSize offset, VkDeviceSize size);

	// post - Unmaps memory
	// future -------------------------------------------- Untie mappies from this object so that there can be more than one?
	void
	unmap();

	//	pre - The addresses written to must be mapped
	//	data - A pointer to the data that will be coppied into the memory
	//	offset - The offset in bytes into the block of memory at which to start copying to
	//	size - The size in bytes of memory that should be coppied
	//	post - This memory should now contain the coppied data
	void
	copyFrom(void *mapping, const void *data, uint32_t offset, size_t size);

	// return - A pointer handle to the device memory
	VkDeviceMemory
	getHandle();

private:
	VkDevice		m_device;
	VkDeviceMemory	m_deviceMemory;
};