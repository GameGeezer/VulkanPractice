#ifndef AMD_VULKAN_SAMPLE_H_
#define AMD_VULKAN_SAMPLE_H_

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

#include "VulkanDevice.h"

class FenceGroup;
class CommandPool;
class CommandBufferGroup;
class RenderPass;
class FrameBuffer;
class VulkanImageView;

namespace AMD
{
	class Window;

	///////////////////////////////////////////////////////////////////////////////
	class VulkanSample
	{
	public:
		VulkanSample(const VulkanSample&) = delete;
		VulkanSample& operator= (const VulkanSample&) = delete;

		VulkanSample();
		virtual ~VulkanSample();

		bool IsInitialized() { return (instance_ != VK_NULL_HANDLE && m_device->getDevice() != VK_NULL_HANDLE); }
		void Run(const int frameCount);
		struct ImportTable;

	protected:
		int GetQueueSlot() const
		{
			return currentBackBuffer_;
		}

		static const uint32_t QUEUE_SLOT_COUNT = 3;

		static int GetQueueSlotCount()
		{
			return QUEUE_SLOT_COUNT;
		}

		VkViewport viewport_;

		VulkanDevice *m_device;

		VkSwapchainKHR swapchain_ = VK_NULL_HANDLE;
		VkInstance instance_ = VK_NULL_HANDLE;

		VkSurfaceKHR surface_ = VK_NULL_HANDLE;

		std::unique_ptr<ImportTable> importTable_;

		//VkFence frameFences_[QUEUE_SLOT_COUNT];
		CommandPool *commandPool;
		CommandBufferGroup *commandBufferGroup;
		FenceGroup *frameFences;
		RenderPass *renderPass;
		VkImage swapchainImages_[QUEUE_SLOT_COUNT];
		VkImageView swapChainImageViews_[QUEUE_SLOT_COUNT];
		VkFramebuffer framebuffer_[QUEUE_SLOT_COUNT];

		std::vector<VulkanImageView*> imageViews;
		std::vector<FrameBuffer*> frameBuffers;

		std::unique_ptr<Window> window_;

		virtual void InitializeImpl(VkCommandBuffer commandBuffer);
		virtual void RenderImpl(VkCommandBuffer commandBuffer);
		virtual void ShutdownImpl();

	private:
		VkCommandBuffer setupCommandBuffer_;
		uint32_t currentBackBuffer_ = 0;

#ifdef _DEBUG
		VkDebugReportCallbackEXT debugCallback_;
#endif
	};

}   // namespace AMD

#endif