#include "MouseClickCallback.h"

#include <vector>
#include <algorithm>

#include "MouseClickListener.h"

MouseClickCallback::MouseClickCallback()
{

}

void
MouseClickCallback::onButtonPress(uint32_t button, float posX, float posY)
{
	for (auto listener = listeners.begin(); listener != listeners.end(); ++listener)
	{
		(*listener)->onMousePress((MouseButton)button, posX, posY);
	}
}

void
MouseClickCallback::onButtonRelease(uint32_t button, float posX, float posY)
{
	for (auto listener = listeners.begin(); listener != listeners.end(); ++listener)
	{
		(*listener)->onMouseRelease((MouseButton)button, posX, posY);
	}
}

void
MouseClickCallback::addListener(MouseClickListener& listener)
{
	listeners.push_back(&listener);
}

void
MouseClickCallback::removeListener(MouseClickListener& listener)
{
	listeners.erase(std::remove(listeners.begin(), listeners.end(), &listener), listeners.end());
}

void
MouseClickCallback::clearListeners()
{
	listeners.clear();
}