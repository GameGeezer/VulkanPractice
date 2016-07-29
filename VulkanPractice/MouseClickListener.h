#pragma once

#include "MouseCodes.h"

class MouseClickListener
{
public:
	virtual void
	onMouseRelease(MouseButton button, float posX, float posY) = 0;

	virtual void
	onMousePress(MouseButton button, float posX, float posY) = 0;
};
