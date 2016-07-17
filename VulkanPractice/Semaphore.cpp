#include "Semaphore.h"

Semaphore::Semaphore(VkDevice device)
{
	VkSemaphoreCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	vkCreateSemaphore(device, &createInfo, nullptr, &semaphore);
}

Semaphore::~Semaphore()
{

}

VkSemaphore*
Semaphore::getSemaphore()
{
	return &semaphore;
}