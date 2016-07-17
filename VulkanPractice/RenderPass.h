#pragma once

#include <vulkan\vulkan.h>

#include <vector>

class RenderSubPass;

class RenderPass
{
public:
	RenderPass();

	~RenderPass();

	void
	initialize();

	void
	addSubPass(RenderSubPass& subPass);

private:
	std::vector<VkSubpassDescription> m_colorAttachments;
};