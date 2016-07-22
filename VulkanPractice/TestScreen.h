#pragma once

#include "Screen.h"

#include <stdint.h>

class VulkanDevice;
class VulkanInstance;
class VulkanCommandPool;
class VulkanCommandBufferGroup;
class VulkanFenceGroup;
class Window;

class TestScreen : public Screen
{
public:
	void
		onCreate() override;

	void
		onPause() override;

	void
		onLeave() override;

	void
		onResume() override;

	void
		onUpdate(uint32_t delta) override;

	void
		onResize(uint32_t width, uint32_t height) override;

	void
		onDestroy() override;
private:
	VulkanDevice					*	m_device;
	VulkanCommandPool				*	m_commandPool;
	VulkanCommandBufferGroup		*	m_commandBufferGroup;
	VulkanFenceGroup				*	m_frameFences;
	Window							*	m_window;
};
