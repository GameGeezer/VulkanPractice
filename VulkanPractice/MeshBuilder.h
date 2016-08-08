#pragma once

#include <stdint.h>
#include <vector>

class Mesh;
class VertexAttribute;
class VulkanDevice;

class MeshBuilder
{
public:
	MeshBuilder();

	~MeshBuilder();

	Mesh*
	build(VulkanDevice &device);

	void
	addAttribute(VertexAttribute& vertexAttribute);

	void
	addVertexData(float *start, uint32_t size);

	void
	addIndices(uint32_t *start, uint32_t size);

	void
	addIndex(uint32_t index);

	uint32_t
	getIndexCount();

private:
	std::vector<VertexAttribute*>	m_vertexAttributes;
	std::vector<float>				m_vertexData;
	std::vector<uint32_t>			m_indices;
};