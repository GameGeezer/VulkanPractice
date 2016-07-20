#include "VulkanQueueSubmission.h"

VulkanQueueSubmission::VulkanQueueSubmission()
{

}

VulkanQueueSubmission::~VulkanQueueSubmission()
{

}

void
VulkanQueueSubmission::inititialize(VkPipelineStageFlags waitDstStageMask)
{
	m_submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	m_submitInfo.commandBufferCount = m_buffersToSubmit.size();
	m_submitInfo.pCommandBuffers = m_buffersToSubmit.data();
	m_submitInfo.signalSemaphoreCount = m_signalSemaphoresToSubmit.size();
	m_submitInfo.pSignalSemaphores = m_signalSemaphoresToSubmit.data();
	m_submitInfo.waitSemaphoreCount = m_waitSemaphoresToSubmit.size();
	m_submitInfo.pWaitSemaphores = m_waitSemaphoresToSubmit.data();
	m_submitInfo.pWaitDstStageMask = &waitDstStageMask;
}

void
VulkanQueueSubmission::addCommandBuffer(VkCommandBuffer commandBuffer)
{
	m_buffersToSubmit.push_back(commandBuffer);
}

void
VulkanQueueSubmission::addSignalSemaphore(VkSemaphore semaphore)
{
	m_signalSemaphoresToSubmit.push_back(semaphore);
}

void
VulkanQueueSubmission::addWaitSemaphore(VkSemaphore semaphore)
{
	m_waitSemaphoresToSubmit.push_back(semaphore);
}

VkSubmitInfo*
VulkanQueueSubmission::getSubmitInfo()
{
	return &m_submitInfo;
}