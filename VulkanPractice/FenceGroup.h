#pragma once

#include <vulkan\vulkan.h>

#include <stdint.h>

class FenceGroup
{
public:
	FenceGroup(uint32_t count, VkDevice device);

	~FenceGroup();

	VkFence*
	getFenceAtIndex(uint32_t index);

	void
	initFenceAtIndex(uint32_t index, bool signaled);

	void
	destroyFenceAtIndex(uint32_t index);

	bool
	getFenceStatusAtIndex(uint32_t index);

	void
	resetFences(uint32_t startIndex, uint32_t count);

	void
	waitForFences(uint32_t startIndex, uint32_t count, VkBool32 waitAll, uint64_t timeout);

private:

	size_t m_count;

	VkFence *m_fences;

	VkDevice m_device;
};