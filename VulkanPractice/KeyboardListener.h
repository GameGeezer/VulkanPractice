#pragma once

class KeyboardListener
{
public:
	virtual void
	onKeyRelease(int keyCode) = 0;

	virtual void
	onKeyPress(int keyCode) = 0;

	virtual void
	onKeyRepeat(int keyCode) = 0;
};