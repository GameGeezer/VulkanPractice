#include "VulkanDevice.h"

#include "VulkanPhysicalDevice.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBufferGroup.h"

VulkanDevice::VulkanDevice(VulkanPhysicalDevice &physicalDevice, uint32_t queueIndex) :  m_physicalDevice(&physicalDevice), m_queueFamilyIndex(queueIndex)
{

}

VulkanDevice::~VulkanDevice()
{
	delete m_commandPool;

	vkDestroyDevice(m_device, nullptr);
}

uint32_t
VulkanDevice::getMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties)
{
	// Iterate over all memory types available for the device used in this example
	uint32_t memoryTypeCount = m_physicalDevice->getMemoryProperties()->memoryTypeCount;
	VkMemoryType *memoryTypes = m_physicalDevice->getMemoryProperties()->memoryTypes;
	for (uint32_t i = 0; i < memoryTypeCount; i++)
	{
		if ((typeBits & 1) == 1)
		{
			if ((memoryTypes[i].propertyFlags & properties) == properties)
			{
				return i;
			}
		}
		typeBits >>= 1;
	}

	throw "Could not find a suitable memory type!";
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

	VkPhysicalDeviceFeatures enabledFeatures = *m_physicalDevice->getFeatures();
	enabledFeatures.shaderClipDistance = VK_TRUE;
	enabledFeatures.shaderCullDistance = VK_TRUE;
	deviceCreateInfo.pEnabledFeatures = &enabledFeatures;

	vkCreateDevice(m_physicalDevice->getHandle(), &deviceCreateInfo, nullptr, &m_device);

	vkGetDeviceQueue(m_device, m_queueFamilyIndex, 0, &m_queue);

	// List all of the available heaps and their properties
	setupEnumerateHeaps();

	m_commandPool = new VulkanCommandPool(m_device, false, true, m_queueFamilyIndex);
}

VkFormat
VulkanDevice::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(m_physicalDevice->getHandle(), format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

VkDeviceMemory
VulkanDevice::allocateMemory(uint32_t typeBits, VkMemoryPropertyFlags properties, VkDeviceSize size)
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
		memoryAllocateInfo.memoryTypeIndex = getMemoryType(typeBits, properties);
		memoryAllocateInfo.allocationSize = size;

		VkDeviceMemory deviceMemory;
		vkAllocateMemory(m_device, &memoryAllocateInfo, nullptr, &deviceMemory);

		return deviceMemory;
	}

	return VK_NULL_HANDLE;
}

void
VulkanDevice::allocateCommandBufferS()
{
	
}

void
VulkanDevice::submitToQueue(uint32_t submitCount, VkSubmitInfo *submitInfo, VkFence fence)
{
	vkQueueSubmit(m_queue, submitCount, submitInfo, fence);
}

void
VulkanDevice::waitOnQueue()
{
	vkQueueWaitIdle(m_queue);
}

VkDevice
VulkanDevice::getHandle()
{
	return m_device;
}

VulkanPhysicalDevice*
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

VulkanCommandPool*
VulkanDevice::getCommandPool()
{
	return m_commandPool;
}

void
VulkanDevice::addExtension(const char* extension)
{
	m_extensions.push_back(extension);
}


void
VulkanDevice::setupEnumerateHeaps()
{
	
	std::vector<MemoryTypeInfo::Heap> heaps;

	uint32_t memoryHeapCount = m_physicalDevice->getMemoryProperties()->memoryHeapCount;
	VkMemoryHeap *memoryHeaps = m_physicalDevice->getMemoryProperties()->memoryHeaps;
	for (uint32_t i = 0; i < memoryHeapCount; ++i)
	{
		MemoryTypeInfo::Heap info;
		info.size = memoryHeaps[i].size;
		info.deviceLocal = (memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) != 0;

		heaps.push_back(info);
	}

	uint32_t memoryTypeCount = m_physicalDevice->getMemoryProperties()->memoryTypeCount;
	VkMemoryType *memoryTypes = m_physicalDevice->getMemoryProperties()->memoryTypes;
	for (uint32_t i = 0; i < memoryTypeCount; ++i)
	{
		MemoryTypeInfo typeInfo;

		typeInfo.deviceLocal = (memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) != 0;
		typeInfo.hostVisible = (memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) != 0;
		typeInfo.hostCoherent = (memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) != 0;
		typeInfo.hostCached = (memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) != 0;
		typeInfo.lazilyAllocated = (memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) != 0;

		typeInfo.heap = heaps[memoryTypes[i].heapIndex];

		typeInfo.index = static_cast<int> (i);

		m_heaps.push_back(typeInfo);
	}
}

VkCommandBuffer
VulkanDevice::beginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_commandPool->getHandle();
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void
VulkanDevice::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(m_queue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(m_queue);

	vkFreeCommandBuffers(m_device, m_commandPool->getHandle(), 1, &commandBuffer);
}