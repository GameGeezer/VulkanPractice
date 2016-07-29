#include "KeyboardCallback.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <algorithm>

#include "KeyboardCallback.h"
#include "KeyboardListener.h"

KeyboardCallback::KeyboardCallback()
{

}

void
KeyboardCallback::onKeyPressed(int32_t keycode)
{
	for (auto listener = listeners.begin(); listener != listeners.end(); ++listener)
	{
		(*listener)->onKeyPress(keycode);
	}
}

void
KeyboardCallback::onKeyReleased(int32_t keycode)
{
	for (auto listener = listeners.begin(); listener != listeners.end(); ++listener)
	{
		(*listener)->onKeyRelease(keycode);
	}
}

void
KeyboardCallback::addListener(KeyboardListener& listener)
{
	listeners.push_back(&listener);
}

void
KeyboardCallback::removeListener(KeyboardListener& listener)
{
	listeners.erase(std::remove(listeners.begin(), listeners.end(), &listener), listeners.end());
}

void
KeyboardCallback::clearListeners()
{
	listeners.clear();
}