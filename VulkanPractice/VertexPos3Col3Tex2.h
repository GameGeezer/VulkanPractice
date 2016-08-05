#pragma once

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

#include "PipelineVertexInputState.h"

class VertexPos3Col3Tex2
{
public:
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoord;

	VertexPos3Col3Tex2();

	VertexPos3Col3Tex2(glm::vec3 position, glm::vec3 color, glm::vec2 texCoord);

	~VertexPos3Col3Tex2();

	static void
	addToPipeline(uint32_t binding, PipelineVertexInputState &vertexInputState)
	{
		vertexInputState.addVertexBindingDescription(binding, VK_VERTEX_INPUT_RATE_VERTEX, sizeof(VertexPos3Col3Tex2));
		vertexInputState.addVertexInputAttributeDescription(binding, VK_FORMAT_R32G32B32_SFLOAT, 0, offsetof(VertexPos3Col3Tex2, position));
		vertexInputState.addVertexInputAttributeDescription(binding, VK_FORMAT_R32G32B32_SFLOAT, 1, offsetof(VertexPos3Col3Tex2, color));
		vertexInputState.addVertexInputAttributeDescription(binding, VK_FORMAT_R32G32_SFLOAT, 2, offsetof(VertexPos3Col3Tex2, texCoord));
	}
};