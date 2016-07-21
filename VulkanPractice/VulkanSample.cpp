#include "VulkanSample.h"

#include <iostream>
#include <algorithm>

#include "Utility.h"
#include "AMDWindow.h"

#include "FenceGroup.h"
#include "CommandPool.h"
#include "CommandBufferGroup.h"
#include "AttachmentDescription.h"
#include "RenderPass.h"
#include "RenderSubPass.h"
#include "FrameBuffer.h"
#include "VulkanImageView.h"
#include "VulkanCommandBuffer.h"

#include "VulkanQueueSubmission.h"

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
		VKAPI_ATTR VkBool32 VKAPI_CALL DebugReportCallback(
			VkDebugReportFlagsEXT       /*flags*/,
			VkDebugReportObjectTypeEXT  /*objectType*/,
			uint64_t                    /*object*/,
			size_t                      /*location*/,
			int32_t                     /*messageCode*/,
			const char*                 /*pLayerPrefix*/,
			const char*                 pMessage,
			void*                       /*pUserData*/)
		{
			OutputDebugStringA(pMessage);
			OutputDebugStringA("\n");
			return VK_FALSE;
		}

		///////////////////////////////////////////////////////////////////////////////
		std::vector<const char*> GetDebugInstanceLayerNames()
		{
			uint32_t layerCount = 0;

			vkEnumerateInstanceLayerProperties(&layerCount,
				nullptr);

			std::vector<VkLayerProperties> instanceLayers{ layerCount };

			vkEnumerateInstanceLayerProperties(&layerCount,
				instanceLayers.data());


			std::vector<const char*> result;
			for (const auto& p : instanceLayers)
			{
				if (strcmp(p.layerName, "VK_LAYER_LUNARG_standard_validation") == 0)
				{
					result.push_back("VK_LAYER_LUNARG_standard_validation");
				}
			}

			return result;
		}

		///////////////////////////////////////////////////////////////////////////////
		std::vector<const char*> GetDebugInstanceExtensionNames()
		{
			uint32_t extensionCount = 0;

			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
				nullptr);

			std::vector<VkExtensionProperties> instanceExtensions{ extensionCount };

			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
				instanceExtensions.data());

			std::vector<const char*> result;
			for (const auto& e : instanceExtensions)
			{
				if (strcmp(e.extensionName, "VK_EXT_debug_report") == 0)
				{
					result.push_back("VK_EXT_debug_report");
				}
			}

			return result;
		}

		///////////////////////////////////////////////////////////////////////////////
		std::vector<const char*> GetDebugDeviceLayerNames(VkPhysicalDevice device)
		{
			uint32_t layerCount = 0;
			vkEnumerateDeviceLayerProperties(device, &layerCount, nullptr);

			std::vector<VkLayerProperties> deviceLayers{ layerCount };
			vkEnumerateDeviceLayerProperties(device, &layerCount, deviceLayers.data());

			std::vector<const char*> result;
			for (const auto& p : deviceLayers)
			{
				if (strcmp(p.layerName, "VK_LAYER_LUNARG_standard_validation") == 0)
				{
					result.push_back("VK_LAYER_LUNARG_standard_validation");
				}
			}

			return result;
		}

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

		struct SwapchainFormatColorSpace
		{
			VkFormat format;
			VkColorSpaceKHR colorSpace;
		};

		///////////////////////////////////////////////////////////////////////////////
		SwapchainFormatColorSpace GetSwapchainFormatAndColorspace(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
		{
			uint32_t surfaceFormatCount = 0;
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, nullptr);

			std::vector<VkSurfaceFormatKHR> surfaceFormats{ surfaceFormatCount };
			vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatCount, surfaceFormats.data());

			SwapchainFormatColorSpace result;

			if (surfaceFormatCount == 1 && surfaceFormats.front().format == VK_FORMAT_UNDEFINED)
			{
				result.format = VK_FORMAT_R8G8B8A8_UNORM;
			}
			else
			{
				result.format = surfaceFormats.front().format;
			}

			result.colorSpace = surfaceFormats.front().colorSpace;

			return result;
		}

		///////////////////////////////////////////////////////////////////////////////
		RenderPass* CreateRenderPass(VkDevice device, VkFormat swapchainFormat)
		{
			AttachmentDescription attachmentDescription(VK_SAMPLE_COUNT_1_BIT, swapchainFormat, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, false);

			VkAttachmentReference attachmentReference = {};
			attachmentReference.attachment = 0;
			attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			RenderSubPass subpass;
			subpass.addColorAttachment(0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
			subpass.initialize();

			RenderPass *renderPass = new RenderPass(device);
			renderPass->addSubPass(subpass);
			renderPass->addAttachmentDescription(attachmentDescription);
			renderPass->initialize();

			return renderPass;
		}

		///////////////////////////////////////////////////////////////////////////////
		void CreateSwapchainImageViews(VkDevice device, VkFormat format,
			const int count, const VkImage* images, VkImageView* imageViews)
		{
			for (int i = 0; i < count; ++i)
			{
				VkImageViewCreateInfo imageViewCreateInfo = {};
				imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				imageViewCreateInfo.image = images[i];
				imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				imageViewCreateInfo.format = format;
				imageViewCreateInfo.subresourceRange.levelCount = 1;
				imageViewCreateInfo.subresourceRange.layerCount = 1;
				imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

				vkCreateImageView(device, &imageViewCreateInfo, nullptr,
					&imageViews[i]);
			}
		}

		///////////////////////////////////////////////////////////////////////////////
		VkSwapchainKHR CreateSwapchain(VkPhysicalDevice physicalDevice, VkDevice device,
			VkSurfaceKHR surface, const int surfaceWidth, const int surfaceHeight,
			const int backbufferCount,
			VkFormat* swapchainFormat)
		{
			VkSurfaceCapabilitiesKHR surfaceCapabilities;
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice,
				surface, &surfaceCapabilities);

			uint32_t presentModeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,
				surface, &presentModeCount, nullptr);

			std::vector<VkPresentModeKHR> presentModes{ presentModeCount };

			vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,
				surface, &presentModeCount, presentModes.data());

			VkExtent2D swapChainSize = {};
			swapChainSize = surfaceCapabilities.currentExtent;
			assert(static_cast<int> (swapChainSize.width) == surfaceWidth);
			assert(static_cast<int> (swapChainSize.height) == surfaceHeight);

			uint32_t swapChainImageCount = backbufferCount;
			assert(swapChainImageCount >= surfaceCapabilities.minImageCount);

			// 0 indicates unlimited number of images
			if (surfaceCapabilities.maxImageCount != 0)
			{
				assert(swapChainImageCount < surfaceCapabilities.maxImageCount);
			}

			VkSurfaceTransformFlagBitsKHR surfaceTransformFlags;

			if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
			{
				surfaceTransformFlags = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			}
			else
			{
				surfaceTransformFlags = surfaceCapabilities.currentTransform;
			}

			auto swapchainFormatColorSpace = GetSwapchainFormatAndColorspace(physicalDevice, surface);

			VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
			swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			swapchainCreateInfo.surface = surface;
			swapchainCreateInfo.minImageCount = swapChainImageCount;
			swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			swapchainCreateInfo.preTransform = surfaceTransformFlags;
			swapchainCreateInfo.imageColorSpace = swapchainFormatColorSpace.colorSpace;
			swapchainCreateInfo.imageFormat = swapchainFormatColorSpace.format;
			swapchainCreateInfo.pQueueFamilyIndices = nullptr;
			swapchainCreateInfo.queueFamilyIndexCount = 0;
			swapchainCreateInfo.clipped = VK_TRUE;
			swapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
			swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			swapchainCreateInfo.imageExtent = swapChainSize;
			swapchainCreateInfo.imageArrayLayers = 1;
			swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;

			VkSwapchainKHR swapchain;
			vkCreateSwapchainKHR(device, &swapchainCreateInfo,
				nullptr, &swapchain);

			if (swapchainFormat)
			{
				*swapchainFormat = swapchainFormatColorSpace.format;
			}

			return swapchain;
		}

		///////////////////////////////////////////////////////////////////////////////
		VkSurfaceKHR CreateSurface(VkInstance instance, HWND hwnd)
		{
			VkWin32SurfaceCreateInfoKHR win32surfaceCreateInfo = {};
			win32surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			win32surfaceCreateInfo.hwnd = hwnd;
			win32surfaceCreateInfo.hinstance = ::GetModuleHandle(nullptr);

			VkSurfaceKHR surface = nullptr;
			vkCreateWin32SurfaceKHR(instance, &win32surfaceCreateInfo, nullptr,
				&surface);

			return surface;
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

		uint32_t physicalDeviceCount = 0;
		vkEnumeratePhysicalDevices(m_instance->getHandle(), &physicalDeviceCount, nullptr);

		std::vector<VkPhysicalDevice> devices{ physicalDeviceCount };
		vkEnumeratePhysicalDevices(m_instance->getHandle(), &physicalDeviceCount, devices.data());

		VkPhysicalDevice physicalDevice = nullptr;
		int graphicsQueueIndex = -1;

		FindPhysicalDeviceWithGraphicsQueue(devices, &physicalDevice, &graphicsQueueIndex);

		m_device = new VulkanDevice(physicalDevice, graphicsQueueIndex);
		m_device->addExtension("VK_KHR_swapchain");
		m_device->initialize();

		window_.reset(new Window{ m_instance->getHandle(), *m_device,"Hello Vulkan", 640, 480, QUEUE_SLOT_COUNT });

		surface_ = window_->getSurface();

		VkBool32 presentSupported;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, 0, surface_, &presentSupported);
		assert(presentSupported);

		uint32_t swapchainImageCount = 0;
		vkGetSwapchainImagesKHR(m_device->getDevice(), window_->getSwapchain(), &swapchainImageCount, nullptr);
		//assert(static_cast<int> (swapchainImageCount) == QUEUE_SLOT_COUNT);

		vkGetSwapchainImagesKHR(m_device->getDevice(), window_->getSwapchain(), &swapchainImageCount, swapchainImages_);

		renderPass = CreateRenderPass(m_device->getDevice(), window_->getSurfaceFormat());

		for (int i = 0; i < QUEUE_SLOT_COUNT; ++i)
		{
			// create imageViews
			VulkanImageView *imageView = new VulkanImageView(m_device->getDevice(), swapchainImages_[i], window_->getSurfaceFormat());
			swapChainImageViews_[i] = imageView->getHandle();
			imageViews.push_back(imageView);

			// create framebuffers
			FrameBuffer *frameBuffer = new FrameBuffer(m_device->getDevice(), *renderPass, swapChainImageViews_[i], window_->getWidth(), window_->getHeight());
			framebuffer_[i] = frameBuffer->getHandle();
			frameBuffers.push_back(frameBuffer);
		}


		commandPool = new CommandPool(m_device->getDevice(), false, true, m_device->getQueueIndex());
		commandBufferGroup = new CommandBufferGroup(m_device->getDevice(), *commandPool, QUEUE_SLOT_COUNT + 1, VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		m_setupCommandBuffer = commandBufferGroup->getCommandBufferAtIndex(QUEUE_SLOT_COUNT);

		frameFences = new FenceGroup(m_device->getDevice(), QUEUE_SLOT_COUNT);

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

		vkDestroyRenderPass(m_device->getDevice(), renderPass->getHandle(), nullptr);

		for (int i = 0; i < imageViews.size(); ++i)
		{
			delete imageViews[i];
		}

		for (int i = 0; i < frameBuffers.size(); ++i)
		{
			delete frameBuffers[i];
		}

		delete commandPool;

		//vkDestroySwapchainKHR(m_device->getDevice(), swapchain_, nullptr);
		vkDestroySurfaceKHR(m_instance->getHandle(), surface_, nullptr);


		vkDestroyDevice(m_device->getDevice(), nullptr);
		vkDestroyInstance(m_instance->getHandle(), nullptr);
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
		//vkWaitForFences(device_, 1, &frameFences_[0], VK_TRUE, UINT64_MAX);

		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkSemaphore imageAcquiredSemaphore;
		vkCreateSemaphore(m_device->getDevice(), &semaphoreCreateInfo, nullptr, &imageAcquiredSemaphore);

		VkSemaphore renderingCompleteSemaphore;
		vkCreateSemaphore(m_device->getDevice(), &semaphoreCreateInfo, nullptr, &renderingCompleteSemaphore);

		for (int i = 0; i < frameCount; ++i)
		{
			vkAcquireNextImageKHR(m_device->getDevice(), window_->getSwapchain(), UINT64_MAX, imageAcquiredSemaphore, VK_NULL_HANDLE, &currentBackBuffer_);

			vkWaitForFences(m_device->getDevice(), 1, frameFences->getPointerToFenceAtIndex(currentBackBuffer_), VK_TRUE, UINT64_MAX);
			vkResetFences(m_device->getDevice(), 1, frameFences->getPointerToFenceAtIndex(currentBackBuffer_));

			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			VulkanCommandBuffer *commandBuffer = commandBufferGroup->getCommandBufferAtIndex(currentBackBuffer_);
			commandBuffer->begin();
			VkCommandBuffer rawCommandBuffer = commandBuffer->getHandle();

			VkClearValue clearValue = {};

			clearValue.color.float32[0] = 0.042f;
			clearValue.color.float32[1] = 0.042f;
			clearValue.color.float32[2] = 0.042f;
			clearValue.color.float32[3] = 1.0f;

			renderPass->begin(rawCommandBuffer, framebuffer_[currentBackBuffer_], window_->getWidth(), window_->getHeight(), clearValue, 1);

			RenderImpl(rawCommandBuffer);

			renderPass->end(rawCommandBuffer);
			commandBuffer->end();

			VulkanQueueSubmission submission;
			submission.addCommandBuffer(rawCommandBuffer);
			submission.addWaitSemaphore(imageAcquiredSemaphore);
			submission.addSignalSemaphore(renderingCompleteSemaphore);
			submission.inititialize(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);
			// Submit rendering work to the graphics queue

			vkQueueSubmit(m_device->getQueue(), 1, submission.getSubmitInfo(), VK_NULL_HANDLE);

			// Submit present operation to present queue
			VkSwapchainKHR sc = window_->getSwapchain();
			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &renderingCompleteSemaphore;
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &sc;
			presentInfo.pImageIndices = &currentBackBuffer_;
			vkQueuePresentKHR(m_device->getQueue(), &presentInfo);

			vkQueueSubmit(m_device->getQueue(), 0, nullptr, frameFences->getFenceAtIndex(currentBackBuffer_));
		};

		// Wait for all rendering to finish
		vkWaitForFences(m_device->getDevice(), 3, frameFences->getPointerToFenceAtIndex(0), VK_TRUE, UINT64_MAX);

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