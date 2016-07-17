#pragma once

#include <vulkan\vulkan.h>

class Semaphore
{
public:

	Semaphore(VkDevice device);

	~Semaphore();

	VkSemaphore*
	getSemaphore();

private:
	VkSemaphore semaphore;
};