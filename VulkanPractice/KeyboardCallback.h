#pragma once

#include <vector>

class KeyboardListener;

using namespace std;

class KeyboardCallback
{
public:

	KeyboardCallback();

	void
	onKeyPressed(int32_t keycode);

	void
	onKeyReleased(int32_t keycode);

	void
	addListener(KeyboardListener& listener);

	void
	removeListener(KeyboardListener& listener);

	void
	clearListeners();

private:
	vector<KeyboardListener*> listeners;
};
