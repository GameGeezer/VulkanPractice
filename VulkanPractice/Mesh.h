#pragma once

#include <vector>
#include <vulkan\vulkan.h>

class VertexAttribute;
class VulkanDevice;
class VulkanDeviceMemory;
class VulkanBuffer;

class Mesh
{
public:

	VulkanDeviceMemory	*	m_meshMemory;
	VulkanBuffer		*	m_vertexBuffer;
	VulkanBuffer		*	m_indexBuffer;

	Mesh(VulkanDevice &device, std::vector<uint32_t>& indices, std::vector<float>& vertexData, std::vector<VertexAttribute*> &vertexAttributes);

	~Mesh();
private:
	VulkanDevice		*	m_device;

};