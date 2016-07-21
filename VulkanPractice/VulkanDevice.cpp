#include "VulkanDevice.h"

VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDevice, uint32_t queueIndex) :  m_physicalDevice(physicalDevice), m_queueFamilyIndex(queueIndex)
{

}

VulkanDevice::~VulkanDevice()
{
	vkDestroyDevice(m_device, nullptr);
}

void
VulkanDevice::initialize()
{
	VkDeviceQueueCreateInfo deviceQueueCreateInfo = {};
	deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueCreateInfo.queueCount = 1;
	deviceQueueCreateInfo.queueFamilyIndex = m_queueFamilyIndex;

	static const float queuePriorities[] = { 1.0f };
	deviceQueueCreateInfo.pQueuePriorities = queuePriorities;

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;

	std::vector<const char*> deviceLayers;
	//Add layers here if needed

	deviceCreateInfo.ppEnabledLayerNames = deviceLayers.data();
	deviceCreateInfo.enabledLayerCount = static_cast<uint32_t> (deviceLayers.size());
	deviceCreateInfo.ppEnabledExtensionNames = m_extensions.data();
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t> (m_extensions.size());

	vkCreateDevice(m_physicalDevice, &deviceCreateInfo, nullptr, &m_device);

	vkGetDeviceQueue(m_device, m_queueFamilyIndex, 0, &m_queue);

	// Get the device properties
	vkGetPhysicalDeviceProperties(m_physicalDevice, &m_physicalDeviceProperties);
	// List all of the available heaps and their properties
	setupEnumerateHeaps();
}

VkDeviceMemory
VulkanDevice::allocateMemory(uint32_t size)
{
	// We take the first HOST_VISIBLE memory
	for (auto& memoryInfo : m_heaps)
	{
		if (!memoryInfo.hostVisible)
		{
			continue;
		}

		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.memoryTypeIndex = memoryInfo.index;
		memoryAllocateInfo.allocationSize = size;

		VkDeviceMemory deviceMemory;
		vkAllocateMemory(m_device, &memoryAllocateInfo, nullptr, &deviceMemory);

		return deviceMemory;
	}

	return VK_NULL_HANDLE;
}

void
VulkanDevice::submitToQueue(uint32_t submitCount, VkSubmitInfo *submitInfo, VkFence fence)
{
	vkQueueSubmit(m_queue, submitCount, submitInfo, fence);
}

VkDevice
VulkanDevice::getDevice()
{
	return m_device;
}

VkPhysicalDevice
VulkanDevice::getPhysicalDevice()
{
	return m_physicalDevice;
}

VkQueue
VulkanDevice::getQueue()
{
	return m_queue;
}

uint32_t
VulkanDevice::getQueueIndex()
{
	return m_queueFamilyIndex;
}

void
VulkanDevice::addExtension(const char* extension)
{
	m_extensions.push_back(extension);
}


void
VulkanDevice::setupEnumerateHeaps()
{
	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_physicalDeviceMemoryProperties);

	std::vector<MemoryTypeInfo::Heap> heaps;

	for (uint32_t i = 0; i < m_physicalDeviceMemoryProperties.memoryHeapCount; ++i)
	{
		MemoryTypeInfo::Heap info;
		info.size = m_physicalDeviceMemoryProperties.memoryHeaps[i].size;
		info.deviceLocal = (m_physicalDeviceMemoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0;

		heaps.push_back(info);
	}

	for (uint32_t i = 0; i < m_physicalDeviceMemoryProperties.memoryTypeCount; ++i)
	{
		MemoryTypeInfo typeInfo;

		typeInfo.deviceLocal = (m_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0;
		typeInfo.hostVisible = (m_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0;
		typeInfo.hostCoherent = (m_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0;
		typeInfo.hostCached = (m_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) != 0;
		typeInfo.lazilyAllocated = (m_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) != 0;

		typeInfo.heap = heaps[m_physicalDeviceMemoryProperties.memoryTypes[i].heapIndex];

		typeInfo.index = static_cast<int> (i);

		m_heaps.push_back(typeInfo);
	}
}