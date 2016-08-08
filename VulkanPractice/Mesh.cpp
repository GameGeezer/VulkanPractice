#include "Mesh.h"

#include "VulkanBuffer.h"
#include "VulkanDevice.h"
#include "VulkanDeviceMemory.h"
#include "VertexAttribute.h"

#include "Utility.h"

Mesh::Mesh(VulkanDevice &device, std::vector<uint32_t>& indices, std::vector<float>& vertexData, std::vector<VertexAttribute*> &vertexAttributes) : m_device(&device)
{
	uint32_t vertexCount = static_cast<uint32_t>(vertexData.size());
	uint32_t indexCount = static_cast<uint32_t>(indices.size());

	uint32_t singleVertexFloatCount = 0;
	for each(VertexAttribute* vertexAttribute in vertexAttributes)
	{
		singleVertexFloatCount += vertexAttribute->getElementsPerVertex();
	}

	uint32_t singleVertexSizeInBytes = singleVertexFloatCount * sizeof(float);

	m_vertexBuffer = new VulkanBuffer(device, static_cast<uint32_t>(vertexCount * sizeof(float)), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
	m_indexBuffer = new VulkanBuffer(device, static_cast<uint32_t>(indexCount * sizeof(uint32_t)), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

	VkDeviceSize vertexBufferSize = m_vertexBuffer->getSize();
	// We want to place the index buffer behind the vertex buffer. Need to take
	// the alignment into account to find the next suitable location
	VkDeviceSize indexBufferOffset = RoundToNextMultiple(vertexBufferSize, m_indexBuffer->getAlignment());


	VkDeviceSize totalBufferSize = indexBufferOffset + m_indexBuffer->getSize();
	auto allocatedDeviceMemory = device.allocateMemory(m_vertexBuffer->getMemoryTypeBits(), VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, totalBufferSize);
	m_meshMemory = new VulkanDeviceMemory(device.getHandle(), allocatedDeviceMemory);
	
	void *mapping = m_meshMemory->map(0, VK_WHOLE_SIZE);
	m_meshMemory->copyFrom(mapping, vertexData.data(), 0, vertexData.size() * sizeof(float));
	m_meshMemory->copyFrom(mapping, indices.data(), static_cast<uint32_t>(indexBufferOffset), indices.size() * sizeof(uint32_t));
	m_meshMemory->unmap();

	m_vertexBuffer->bindToMemory(m_meshMemory->getHandle(), 0);
	m_indexBuffer->bindToMemory(m_meshMemory->getHandle(), static_cast<uint32_t>(indexBufferOffset));
	
}

Mesh::~Mesh()
{

}