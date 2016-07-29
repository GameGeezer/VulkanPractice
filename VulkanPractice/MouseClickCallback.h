#pragma once

#include <vector>

class MouseClickListener;

using namespace std;

class MouseClickCallback
{
public:

	MouseClickCallback();

	void
	onButtonPress(uint32_t button, float posX, float posY);

	void
	onButtonRelease(uint32_t button, float posX, float posY);

	void
	addListener(MouseClickListener& listener);

	void
	removeListener(MouseClickListener& listener);

	void
	clearListeners();

private:
	vector<MouseClickListener*> listeners;
};
