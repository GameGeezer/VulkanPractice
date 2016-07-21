#ifndef AMD_VULKAN_SAMPLE_H_
#define AMD_VULKAN_SAMPLE_H_

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>

#include "VulkanDevice.h"
#include "VulkanInstance.h"

class VulkanFenceGroup;
class CommandPool;
class VulkanCommandBufferGroup;
class VulkanRenderPass;
class VulkanFrameBuffer;
class VulkanImageView;
class VulkanCommandBuffer;
class VulkanInstance;

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

		bool IsInitialized() { return (m_instance->getHandle() != VK_NULL_HANDLE && m_device->getDevice() != VK_NULL_HANDLE); }
		void Run(const int frameCount);

	protected:
		int GetQueueSlot() const
		{
			return currentBackBuffer_;
		}

		static const uint32_t QUEUE_SLOT_COUNT = 2;

		static int GetQueueSlotCount()
		{
			return QUEUE_SLOT_COUNT;
		}

		VulkanDevice *m_device;

		VulkanInstance *m_instance;

		//VkFence frameFences_[QUEUE_SLOT_COUNT];
		CommandPool *commandPool;
		VulkanCommandBufferGroup *commandBufferGroup;
		VulkanFenceGroup *frameFences;

		std::unique_ptr<Window> window_;

		virtual void InitializeImpl(VkCommandBuffer commandBuffer);
		virtual void RenderImpl(VkCommandBuffer commandBuffer);
		virtual void ShutdownImpl();

	private:
		VulkanCommandBuffer *m_setupCommandBuffer;
		uint32_t currentBackBuffer_ = 0;
	};

}   // namespace AMD

#endif