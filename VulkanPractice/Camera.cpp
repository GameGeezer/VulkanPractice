#include "Camera.h"

#include <math.h>

#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

Camera::Camera(float width, float height, float near, float far, float fieldOfView) : width(width), height(height), nearView(near), farView(far), fieldOfView(fieldOfView), direction(0, 0, -1)
{
	updateProjection();
	updateView();
}


Camera::~Camera()
{

}

void
Camera::moveForward(float amount)
{
	position += direction * amount;
	updateView();
}

void
Camera::moveBackward(float amount)
{
	position -= direction * amount;
	updateView();
}

void
Camera::moveLeft(float amount)
{
	position -= glm::cross(direction, glm::vec3(0, 1, 0)) * amount;
	updateView();
}

void
Camera::moveRight(float amount)
{
	position += glm::cross(direction, glm::vec3(0, 1, 0)) * amount;
	updateView();
}

void
Camera::translate(float x, float y, float z)
{
	position += glm::vec3(x, y, z);
	updateView();
}

void
Camera::rotate(float amount, glm::vec3 axis)
{
	direction = glm::rotate(direction, amount, axis);
	updateView();
}

void
Camera::rotateLocalX(float ammount)
{
	rotate(ammount, glm::cross(direction, glm::vec3(0, 1, 0)));
}

void
Camera::constructViewProjection(glm::mat4& target)
{
	target = projection * view;
}

void
Camera::constructInverseViewProjection(glm::mat4& target)
{
	constructViewProjection(target);

	target = glm::inverse(target);
}

void
Camera::updateProjection()
{
	float aspectRatio = width / height;
	projection = glm::perspective(fieldOfView, aspectRatio, nearView, farView);
}

void
Camera::updateView()
{
	view = glm::lookAt(position, position + direction, glm::vec3(0, 1, 0));
	//view = glm::lookAt(position, position + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
}

glm::mat4*
Camera::getProjection()
{
	return &projection;
}

glm::mat4*
Camera::getView()
{
	return &view;
}

glm::vec3
Camera::getPosition()
{
	return position;
}

glm::vec3
Camera::getDirection()
{
	return direction;
}
