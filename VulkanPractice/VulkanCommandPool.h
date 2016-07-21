#pragma once

#include <vulkan\vulkan.h>

class CommandPool
{
public:
	CommandPool(VkDevice device, bool transient, bool individual, uint32_t queueFamilyIndex);

	~CommandPool();

	void
	reset(bool releaseResources);

	VkCommandPool
	getHandle();

private:
	VkDevice		m_device;
	VkCommandPool	m_commandPool;
};