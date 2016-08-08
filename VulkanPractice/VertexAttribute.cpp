#include "VertexAttribute.h"

VertexAttribute::VertexAttribute(uint32_t elementsPerVertex): m_elementsPerVertex(elementsPerVertex)
{

}

VertexAttribute::~VertexAttribute()
{

}

uint32_t
VertexAttribute::getElementsPerVertex()
{
	return m_elementsPerVertex;
}