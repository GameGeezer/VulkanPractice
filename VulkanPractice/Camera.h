#pragma once

#include <glm/mat4x4.hpp>
#include <glm\vec3.hpp>

class Camera
{
public:
	Camera(float width, float height, float nearView, float farView, float fieldOfView);

	~Camera();

	void
	moveForward(float amount);

	void
	moveBackward(float amount);

	void
	moveLeft(float amount);

	void
	moveRight(float amount);

	void
	translate(float x, float y, float z);

	void
	rotate(float ammount, glm::vec3 axis);

	void
	rotateLocalX(float ammount);

	void
	constructViewProjection(glm::mat4& target);

	void
	constructInverseViewProjection(glm::mat4& target);

	glm::mat4*
	getProjection();

	glm::mat4*
	getView();

	 glm::vec3
	getPosition();

	 glm::vec3
	getDirection();

private:
	void
	updateProjection();

	void
	updateView();

	float width, height, nearView, farView, fieldOfView;
	glm::mat4 projection, view;
	glm::vec3 position, direction, up;
};