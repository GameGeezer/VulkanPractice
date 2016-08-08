#pragma once

#include <stdint.h>

class VertexAttribute
{
public:
	VertexAttribute(uint32_t elementsPerVertex);

	~VertexAttribute();

	uint32_t
	getElementsPerVertex();

private:
	uint32_t m_elementsPerVertex;
};