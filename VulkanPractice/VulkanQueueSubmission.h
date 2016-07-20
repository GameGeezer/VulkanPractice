#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class VulkanQueueSubmission
{
public:
	VulkanQueueSubmission();

	~VulkanQueueSubmission();

	void
	inititialize(VkPipelineStageFlags waitDstStageMask);
	
	void
	addCommandBuffer(VkCommandBuffer commandBuffer);

	void
	addSignalSemaphore(VkSemaphore semaphore);

	void
	addWaitSemaphore(VkSemaphore semaphore);

	VkSubmitInfo*
	getSubmitInfo();

private:
	VkSubmitInfo						m_submitInfo = {};
	std::vector<VkCommandBuffer>		m_buffersToSubmit;
	std::vector<VkSemaphore>			m_signalSemaphoresToSubmit;
	std::vector<VkSemaphore>			m_waitSemaphoresToSubmit;
};