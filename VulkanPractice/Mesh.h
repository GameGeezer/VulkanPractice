#pragma once

#include <vector>

template<class T>
class Mesh
{
public:

	void
	addVertices(std::vector<T> vertices);

	void
	addindices(std::vector<uint32_t> indices);

	size_t
	getVertexCount();

	size_t
	getIndexCount();

private:
	std::vector<T>			m_vertices;
	std::vector<uint32_t>	m_indices;
};