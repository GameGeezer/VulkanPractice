#include "VulkanSample.h"

#include <iostream>
#include <algorithm>

#include "Utility.h"
#include "Window.h"

#include "VulkanFenceGroup.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBufferGroup.h"
#include "VulkanAttachmentDescription.h"
#include "VulkanRenderPass.h"
#include "VulkanRenderSubPass.h"
#include "VulkanImageView.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSemaphore.h"

#include "VulkanQueueSubmission.h"

#include "VulkanPhysicalDevice.h"

#include "VulkanEnumeratedPhysicalDevices.h"

#include <cassert>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#pragma warning( disable : 4100 ) // disable unreferenced formal parameter warnings

namespace AMD
{

	namespace
	{
		///////////////////////////////////////////////////////////////////////////////
		void FindPhysicalDeviceWithGraphicsQueue(const std::vector<VkPhysicalDevice>& physicalDevices,
			VkPhysicalDevice* outputDevice, int* outputGraphicsQueueIndex)
		{
			for (auto physicalDevice : physicalDevices)
			{
				uint32_t queueFamilyPropertyCount = 0;

				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
					&queueFamilyPropertyCount, nullptr);

				std::vector<VkQueueFamilyProperties> queueFamilyProperties{ queueFamilyPropertyCount };
				vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice,
					&queueFamilyPropertyCount, queueFamilyProperties.data());

				int i = 0;
				for (const auto& queueFamilyProperty : queueFamilyProperties)
				{
					if (queueFamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT)
					{
						if (outputDevice)
						{
							*outputDevice = physicalDevice;
						}

						if (outputGraphicsQueueIndex)
						{
							*outputGraphicsQueueIndex = i;
						}

						return;
					}

					++i;
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////
		VulkanRenderPass* CreateRenderPass(VkDevice device, VkFormat swapchainFormat)
		{
			VulkanAttachmentDescription attachmentDescription(VK_SAMPLE_COUNT_1_BIT, swapchainFormat, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, false);

			VkAttachmentReference attachmentReference = {};
			attachmentReference.attachment = 0;
			attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VulkanRenderSubPass subpass;
			subpass.addColorAttachment(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			subpass.initialize();

			VulkanRenderPass *renderPass = new VulkanRenderPass(device);
			renderPass->addSubPass(subpass);
			renderPass->addAttachmentDescription(*(attachmentDescription.getDescription()));
			renderPass->initialize();

			return renderPass;
		}

	}   // namespace

		///////////////////////////////////////////////////////////////////////////////
	VulkanSample::VulkanSample()
	{
		m_instance = new VulkanInstance();
		m_instance->addExtension("VK_KHR_surface");
		m_instance->addExtension("VK_KHR_win32_surface");

		if (!m_instance->instantiate())
		{
			// just bail out if the user does not have a compatible Vulkan driver
			return;
		}

		enumeratedPhysicalDevices = new VulkanEnumeratedPhysicalDevices(m_instance->getHandle());
		std::vector<VulkanPhysicalDevice*> physicalDevices;
		std::vector<uint32_t> queueIndexes;
		enumeratedPhysicalDevices->findDevicesWithQueueFlag(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT, physicalDevices, queueIndexes);

		VkPhysicalDevice physicalDevice = physicalDevices.at(0)->getHandle();
		int graphicsQueueIndex = queueIndexes.at(0);

		m_device = new VulkanDevice(physicalDevice, graphicsQueueIndex);
		m_device->addExtension("VK_KHR_swapchain");
		m_device->initialize();

		m_window = new Window( m_instance->getHandle(), *m_device,"Hello Vulkan", 640, 480, QUEUE_SLOT_COUNT );

		commandPool = new VulkanCommandPool(m_device->getDevice(), false, true, m_device->getQueueIndex());
		commandBufferGroup = new VulkanCommandBufferGroup(m_device->getDevice(), commandPool->getHandle(), QUEUE_SLOT_COUNT + 1, VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		m_setupCommandBuffer = commandBufferGroup->getCommandBufferAtIndex(QUEUE_SLOT_COUNT);

		frameFences = new VulkanFenceGroup(m_device->getDevice(), QUEUE_SLOT_COUNT);

		for (int i = 0; i < QUEUE_SLOT_COUNT; ++i)
		{
			frameFences->initFenceAtIndex(i, true);
		}
	}

	///////////////////////////////////////////////////////////////////////////////
	VulkanSample::~VulkanSample()
	{
	}

	///////////////////////////////////////////////////////////////////////////////
	void VulkanSample::ShutdownImpl()
	{
		for (int i = 0; i < QUEUE_SLOT_COUNT; ++i)
		{
			frameFences->destroyFenceAtIndex(i);
		}

		delete commandPool;

		delete m_window;

		delete m_device;
		
		delete m_instance;
	}

	///////////////////////////////////////////////////////////////////////////////
	void VulkanSample::Run(const int frameCount)
	{
		if (IsInitialized() == false)
		{
			// just bail out if the user does not have a compatible Vulkan driver
			return;
		}
		frameFences->resetFences(0, 1);
		{
			// Start recording to a command buffer
			m_setupCommandBuffer->begin();

			InitializeImpl(m_setupCommandBuffer->getHandle());
			// Finish recording to a command buffer
			m_setupCommandBuffer->end();
			// What to submit to the queue
			VkCommandBuffer buf = m_setupCommandBuffer->getHandle();
			VulkanQueueSubmission submission;
			submission.addCommandBuffer(buf);
			submission.inititialize(0);
			// Submit the command buffer to the queue
			m_device->submitToQueue(1, submission.getSubmitInfo(), frameFences->getFenceAtIndex(0));
		}
		// Wait for queue to finish
		frameFences->waitForFences(0, 1, VK_TRUE, UINT64_MAX);

		VulkanSemaphore imageAcquiredSemaphoreWrapper(m_device->getDevice());
		VkSemaphore imageAcquiredSemaphore = imageAcquiredSemaphoreWrapper.getHandle();

		VulkanSemaphore renderingCompleteSemaphoreWrapper(m_device->getDevice());
		VkSemaphore renderingCompleteSemaphore = renderingCompleteSemaphoreWrapper.getHandle();

		for (int i = 0; i < frameCount; ++i)
		{
			vkAcquireNextImageKHR(m_device->getDevice(), m_window->getSwapchain(), UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &currentBackBuffer_);

			frameFences->waitForFences(currentBackBuffer_, 1, VK_TRUE, UINT64_MAX);
			frameFences->resetFences(currentBackBuffer_, 1);

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
			m_device->submitToQueue(1, submission.getSubmitInfo(), VK_NULL_HANDLE);

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

			m_device->submitToQueue(0, nullptr, frameFences->getFenceAtIndex(currentBackBuffer_));
		};

		// Wait for all rendering to finish
		vkWaitForFences(m_device->getDevice(), 2, frameFences->getPointerToFenceAtIndex(0), VK_TRUE, UINT64_MAX);

		vkDestroySemaphore(m_device->getDevice(), imageAcquiredSemaphore, nullptr);
		vkDestroySemaphore(m_device->getDevice(), renderingCompleteSemaphore, nullptr);

		ShutdownImpl();
	}

	///////////////////////////////////////////////////////////////////////////////
	void VulkanSample::InitializeImpl(VkCommandBuffer /*commandBuffer*/)
	{
	}

	///////////////////////////////////////////////////////////////////////////////
	void VulkanSample::RenderImpl(VkCommandBuffer /*commandBuffer*/)
	{
	}

}   // namespace AMD