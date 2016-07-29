#pragma once

#include "KeyboardListener.h"
#include "MouseClickListener.h"
#include "MouseMoveListener.h"

#include <stdint.h>

#include <glm\vec2.hpp>

class Camera;

class Player : public KeyboardListener, public MouseClickListener, public MouseMoveListener
{
public:
	Player();

	~Player();

	void
	update(uint32_t delta);

	void
	onKeyRelease(int32_t keyCode) override;

	void
	onKeyPress(int32_t keyCode) override;

	void
	onKeyRepeat(int32_t keyCode) override;

	void
	onMouseRelease(MouseButton button, float posX, float posY) override;

	void
	onMousePress(MouseButton button, float posX, float posY) override;

	void
	onMouseMove(float posX, float posY) override;

	Camera*
	getCamera();

private:
	Camera	*	m_camera;

	glm::vec2	m_lastMousePosition, m_lastMouseClickPosition;
	bool		m_isWPressed = false, m_isAPressed = false, m_isSPressed = false, m_isDPressed = false, m_isLeftMousePressed = false, m_isRightMousePressed = false;
};