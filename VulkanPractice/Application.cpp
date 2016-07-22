#include "Application.h"

#include "Game.h"

#include <vulkan\vulkan.h>

#include <vector>

#include "Window.h"
#include "VulkanInstance.h"
#include "VulkanEnumeratedPhysicalDevices.h"
#include "VulkanDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBufferGroup.h"
#include "VulkanFenceGroup.h"
#include "VulkanQueueSubmission.h"
#include "VulkanCommandBuffer.h"


Application::Application(Game& game, string windowTitle, int windowWidth, int windowHeight) : m_game(&game), m_windowTitle(windowTitle), m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
	// Create instance

	m_instance = new VulkanInstance();
	m_instance->addExtension("VK_KHR_surface");
	m_instance->addExtension("VK_KHR_win32_surface");

	if (!m_instance->instantiate())
	{
		return;
	}

	// List all physical devices on the machine

	VulkanEnumeratedPhysicalDevices enumeratedPhysicalDevices(m_instance->getHandle());
	std::vector<VulkanPhysicalDevice*> physicalDevices;
	std::vector<uint32_t> queueIndexes;
	enumeratedPhysicalDevices.findDevicesWithQueueFlag(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT, physicalDevices, queueIndexes);

	if (physicalDevices.size() == 0)
	{
		return;
	}

	// Set up the virtual device that we're going to use

	VkPhysicalDevice physicalDevice = physicalDevices.at(0)->getHandle();
	uint32_t graphicsQueueIndex = queueIndexes.at(0);

	m_device = new VulkanDevice(physicalDevice, graphicsQueueIndex);
	m_device->addExtension("VK_KHR_swapchain");
	m_device->initialize();

	m_window = new Window(m_instance->getHandle(), *m_device, "Hello Vulkan", m_windowWidth, m_windowHeight, QUEUE_SLOT_COUNT);

	m_commandPool = new VulkanCommandPool(m_device->getDevice(), false, true, m_device->getQueueIndex());

	m_commandBufferGroup = new VulkanCommandBufferGroup(m_device->getDevice(), m_commandPool->getHandle(), QUEUE_SLOT_COUNT + 1, VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY);

	m_setupCommandBuffer = m_commandBufferGroup->getCommandBufferAtIndex(QUEUE_SLOT_COUNT);

	m_fences = new VulkanFenceGroup(m_device->getDevice(), QUEUE_SLOT_COUNT);

	for (uint32_t i = 0; i < QUEUE_SLOT_COUNT; ++i)
	{
		m_fences->initFenceAtIndex(i, true);
	}
}

Application::~Application()
{
	delete m_fences;

	delete m_commandPool;

	delete m_commandBufferGroup;

	delete m_window;

	delete m_device;

	delete m_instance;
}

void
Application::start()
{
	this->init();

	this->m_game->onCreate();

	this->loop();
}

void
Application::init()
{
	m_fences->resetFences(0, 1);
	{
		// Start recording to a command buffer
		m_setupCommandBuffer->begin();

		//InitializeImpl(m_setupCommandBuffer->getHandle());------------------------------------------------------------------------------------ERROR
		// Finish recording to a command buffer
		m_setupCommandBuffer->end();
		// What to submit to the queue
		VkCommandBuffer buf = m_setupCommandBuffer->getHandle();
		VulkanQueueSubmission submission;
		submission.addCommandBuffer(buf);
		submission.inititialize(0);
		// Submit the command buffer to the queue
		m_device->submitToQueue(1, submission.getSubmitInfo(), m_fences->getFenceAtIndex(0));
	}
	// Wait for queue to finish
	m_fences->waitForFences(0, 1, VK_TRUE, UINT64_MAX);
	//vkWaitForFences(device_, 1, &frameFences_[0], VK_TRUE, UINT64_MAX);

	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkSemaphore imageAcquiredSemaphore;
	vkCreateSemaphore(m_device->getDevice(), &semaphoreCreateInfo, nullptr, &imageAcquiredSemaphore);

	VkSemaphore renderingCompleteSemaphore;
	vkCreateSemaphore(m_device->getDevice(), &semaphoreCreateInfo, nullptr, &renderingCompleteSemaphore);
}

void
Application::loop()
{
	/*
	vkAcquireNextImageKHR(m_device->getDevice(), m_window->getSwapchain(), UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &currentBackBuffer_);

	vkWaitForFences(m_device->getDevice(), 1, frameFences->getPointerToFenceAtIndex(currentBackBuffer_), VK_TRUE, UINT64_MAX);
	vkResetFences(m_device->getDevice(), 1, frameFences->getPointerToFenceAtIndex(currentBackBuffer_));

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

	VulkanCommandBuffer *commandBuffer = commandBufferGroup->getCommandBufferAtIndex(currentBackBuffer_);
	commandBuffer->begin();
	VkCommandBuffer rawCommandBuffer = commandBuffer->getHandle();

	m_window->beginRenderPass(rawCommandBuffer, currentBackBuffer_);

	RenderImpl(rawCommandBuffer);

	m_window->endRenderPass(rawCommandBuffer);
	commandBuffer->end();

	VulkanQueueSubmission submission;
	submission.addCommandBuffer(rawCommandBuffer);
	submission.addWaitSemaphore(imageAcquiredSemaphore);
	submission.addSignalSemaphore(renderingCompleteSemaphore);
	submission.inititialize(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
	// Submit rendering work to the graphics queue

	vkQueueSubmit(m_device->getQueue(), 1, submission.getSubmitInfo(), VK_NULL_HANDLE);

	// Submit present operation to present queue
	VkSwapchainKHR sc = m_window->getSwapchain();
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderingCompleteSemaphore;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &sc;
	presentInfo.pImageIndices = &currentBackBuffer_;
	vkQueuePresentKHR(m_device->getQueue(), &presentInfo);

	vkQueueSubmit(m_device->getQueue(), 0, nullptr, frameFences->getFenceAtIndex(currentBackBuffer_));
	*/
}