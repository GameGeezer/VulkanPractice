#include "MeshBuilder.h"

#include "Mesh.h"

MeshBuilder::MeshBuilder()
{

}

MeshBuilder::~MeshBuilder()
{

}

Mesh*
MeshBuilder::build(VulkanDevice &device)
{
	return new Mesh(device, m_indices, m_vertexData, m_vertexAttributes);
}

void
MeshBuilder::addAttribute(VertexAttribute& vertexAttribute)
{
	m_vertexAttributes.push_back(&vertexAttribute);
}

void
MeshBuilder::addVertexData(float *start, uint32_t size)
{
	m_vertexData.insert(m_vertexData.end(), start, start + size);
}

void
MeshBuilder::addIndices(uint32_t *start, uint32_t size)
{
	m_indices.insert(m_indices.end(), start, start + size);
}

void
MeshBuilder::addIndex(uint32_t index)
{
	m_indices.push_back(index);
}

uint32_t
MeshBuilder::getIndexCount()
{
	return static_cast<uint32_t>(m_indices.size());
}