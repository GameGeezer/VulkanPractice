#pragma once

#include <vulkan\vulkan.h>

#include <vector>

using namespace std;

struct MemoryTypeInfo
{
	bool deviceLocal = false;
	bool hostVisible = false;
	bool hostCoherent = false;
	bool hostCached = false;
	bool lazilyAllocated = false;

	struct Heap
	{
		uint64_t size = 0;
		bool deviceLocal = false;
	};

	Heap heap;
	int index;
};

class VulkanDevice
{
public:

	VulkanDevice(VkPhysicalDevice physicalDevice, uint32_t graphicsQueueIndex, std::vector<const char*> &extensions);

	~VulkanDevice();

	VkDeviceMemory
	allocateMemory(uint32_t size);

	VkDevice
	getDevice();

	VkPhysicalDevice
	getPhysicalDevice();

	VkQueue
	getQueue();

	uint32_t
		getQueueIndex();

private:
	VkDevice							m_device;
	VkPhysicalDevice					m_physicalDevice;
	VkPhysicalDeviceProperties			m_physicalDeviceProperties;
	VkPhysicalDeviceMemoryProperties	m_physicalDeviceMemoryProperties;

	VkQueue								m_queue;
	uint32_t							m_queueFamilyIndex;

	std::vector<MemoryTypeInfo>			m_heaps;

	void
	setupEnumerateHeaps();
};