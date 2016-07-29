#pragma once

#include <vector>

class MouseMoveListener;

using namespace std;

class MouseMoveCallback
{
public:

	MouseMoveCallback();

	void
	invoke(float xpos, float ypos);

	void
	addListener(MouseMoveListener& listener);

	void
	removeListener(MouseMoveListener& listener);

	void
	clearListeners();

private:
	vector<MouseMoveListener*> listeners;
};