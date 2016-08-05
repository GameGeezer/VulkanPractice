#include "Mesh.h"

template<class T>
void
Mesh<T>::addVertices(std::vector<T> vertices)
{
	m_vertices.push_back(vertices);
}

template<class T>
void
Mesh<T>::addindices(std::vector<uint32_t> indices)
{
	m_indices.push_back(indices);
}

template<class T>
size_t
Mesh<T>::getVertexCount()
{
	return m_vertices.size();
}

template<class T>
size_t
Mesh<T>::getIndexCount()
{
	return m_indices.size();
}