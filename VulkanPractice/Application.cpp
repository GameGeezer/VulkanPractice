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
#include "VulkanSemaphore.h"


Application::Application(Game& game, string windowTitle, int windowWidth, int windowHeight) : m_game(&game), m_windowTitle(windowTitle), m_windowWidth(windowWidth), m_windowHeight(windowHeight)
{
	
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

	this->m_game->onCreate(*this);

	this->loop();
}

void
Application::init()
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

void
Application::loop()
{
	VulkanSemaphore imageAcquiredSemaphoreWrapper(m_device->getDevice());
	VkSemaphore imageAcquiredSemaphore = imageAcquiredSemaphoreWrapper.getHandle();

	VulkanSemaphore renderingCompleteSemaphoreWrapper(m_device->getDevice());
	VkSemaphore renderingCompleteSemaphore = renderingCompleteSemaphoreWrapper.getHandle();

	while (m_window->isAlive())
	{
		uint32_t currentBackBuffer = m_window->acquireNextImage(UINT64_MAX, imageAcquiredSemaphore);
		m_fences->waitForFences(currentBackBuffer, 1, VK_TRUE, UINT64_MAX);
		m_fences->resetFences(currentBackBuffer, 1);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		VulkanCommandBuffer *commandBuffer = m_commandBufferGroup->getCommandBufferAtIndex(currentBackBuffer);
		commandBuffer->begin();
		VkCommandBuffer rawCommandBuffer = commandBuffer->getHandle();

		m_window->beginRenderPass(rawCommandBuffer, currentBackBuffer);

		m_game->render(commandBuffer);

		m_window->endRenderPass(rawCommandBuffer);
		commandBuffer->end();

		VulkanQueueSubmission submission;
		submission.addCommandBuffer(rawCommandBuffer);
		submission.addWaitSemaphore(imageAcquiredSemaphore);
		submission.addSignalSemaphore(renderingCompleteSemaphore);
		submission.inititialize(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
		// Submit rendering work to the graphics queue
		m_device->submitToQueue(1, submission.getSubmitInfo(), VK_NULL_HANDLE);
		// Submit present operation to present queue
		VkSwapchainKHR sc = m_window->getSwapchain();
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &renderingCompleteSemaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &sc;
		presentInfo.pImageIndices = &currentBackBuffer;
		vkQueuePresentKHR(m_device->getQueue(), &presentInfo);

		m_device->submitToQueue(0, nullptr, m_fences->getFenceAtIndex(currentBackBuffer));
	}
}

VulkanInstance*
Application::getInstance()
{
	return m_instance;
}

VulkanDevice*
Application::getDevice()
{
	return m_device;
}

VulkanFenceGroup*
Application::getFences()
{
	return m_fences;
}

Window*
Application::getWindow()
{
	return m_window;
}

VulkanCommandBufferGroup*
Application::getCommandBuffers()
{
	return m_commandBufferGroup;
}