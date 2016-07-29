#include "Player.h"

#include "Window.h"
#include "Camera.h"

#include "KeyboardKeycodes.h"

Player::Player()
{
	Window::KEYBOARD->addListener(*this);
	Window::MOUSE_CLICK->addListener(*this);
	Window::MOUSE_MOVE->addListener(*this);

	m_camera = new Camera(640, 680, 0.1f, 100.0f, 45.0f);
}

Player::~Player()
{

}

void
Player::update(uint32_t delta)
{
	if (m_isWPressed)
		m_camera->moveForward(0.04f);
	if (m_isAPressed)
		m_camera->moveLeft(0.04f);
	if (m_isSPressed)
		m_camera->moveBackward(0.04f);
	if (m_isDPressed)
		m_camera->moveRight(0.04f);
}

void
Player::onKeyRelease(int32_t keyCode)
{
	if (keyCode == KeyboardKeycode::KEY_W)
		m_isWPressed = false;
	if (keyCode == KeyboardKeycode::KEY_A)
		m_isAPressed = false;
	if (keyCode == KeyboardKeycode::KEY_S)
		m_isSPressed = false;
	if (keyCode == KeyboardKeycode::KEY_D)
		m_isDPressed = false;
}

void
Player::onKeyPress(int32_t keyCode)
{
	if (keyCode == KeyboardKeycode::KEY_W)
		m_isWPressed = true;
	if (keyCode == KeyboardKeycode::KEY_A)
		m_isAPressed = true;
	if (keyCode == KeyboardKeycode::KEY_S)
		m_isSPressed = true;
	if (keyCode == KeyboardKeycode::KEY_D)
		m_isDPressed = true;
}

void
Player::onKeyRepeat(int32_t keyCode)
{

}

void
Player::onMouseRelease(MouseButton button, float posX, float posY)
{
	if (button == MouseButton::LEFT)
	{
		m_isLeftMousePressed = false;
	}
	if (button == MouseButton::RIGHT)
	{
		m_isRightMousePressed = false;
	}
}

void
Player::onMousePress(MouseButton button, float posX, float posY)
{
	m_lastMousePosition = glm::vec2(posX, posY);
	m_lastMouseClickPosition = glm::vec2(posX, posY);

	if (button == MouseButton::LEFT)
	{
		m_isLeftMousePressed = true;
	}

	if (button == MouseButton::RIGHT)
	{
		m_isRightMousePressed = true;
	}
}

void
Player::onMouseMove(float posX, float posY)
{
	if (m_isLeftMousePressed)
	{
		m_camera->rotate(0.01f * (m_lastMousePosition.x - posX), glm::vec3(0, 1, 0));
		m_camera->rotateLocalX(0.01f * (m_lastMousePosition.y - posY));
	}

	m_lastMousePosition = glm::vec2(posX, posY);
}

Camera*
Player::getCamera()
{
	return m_camera;
}