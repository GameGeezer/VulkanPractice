#pragma once

#include <vulkan\vulkan.h>

#include <vector>

using namespace std;

class VulkanPhysicalDevice;
class VulkanCommandPool;

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

	VulkanDevice(VulkanPhysicalDevice &physicalDevice, uint32_t graphicsQueueIndex);

	~VulkanDevice();

	void
	initialize();

	uint32_t
	getMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties);

	VkDeviceMemory
	allocateMemory(uint32_t typeBits, VkMemoryPropertyFlags properties, VkDeviceSize size);

	void
	submitToQueue(uint32_t submitCount, VkSubmitInfo *submitInfo, VkFence fence);

	void
	waitOnQueue();

	VkDevice
	getDevice();

	VulkanPhysicalDevice*
	getPhysicalDevice();

	VkQueue
	getQueue();

	uint32_t
	getQueueIndex();

	VulkanCommandPool*
	getCommandPool();

	void
	addExtension(const char* extension);

private:
	VkDevice							m_device;
	VulkanPhysicalDevice			*	m_physicalDevice;

	VkQueue								m_queue;
	uint32_t							m_queueFamilyIndex;

	VulkanCommandPool				*	m_commandPool;

	std::vector<MemoryTypeInfo>			m_heaps;
	std::vector<const char*>			m_extensions;

	void
	setupEnumerateHeaps();
};