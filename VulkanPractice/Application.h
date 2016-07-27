#ifndef VALIS_APPLICATION_H
#define VALIS_APPLICATION_H

#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <vector>

//#include "KeyboardCallback.cuh"
//#include "MouseClickCallback.cuh"
//#include "MouseMoveCallback.cuh"

class Game;
class VulkanInstance;
class VulkanDevice;
class Window;
class VulkanCommandPool;
class VulkanCommandBufferGroup;
class VulkanFenceGroup;
class VulkanCommandBuffer;

using namespace std;
using namespace std::chrono;

class Application
{
public:
	//static KeyboardCallback* KEYBOARD;
	//static MouseClickCallback* MOUSE_CLICK;
	//static MouseMoveCallback* MOUSE_MOVE;

	Application(Game& game, string windowTitle, int windowWidth, int windowHeight);
	~Application();

	void
	start();

	VulkanInstance*
	getInstance();

	VulkanDevice*
	getDevice();

	VulkanFenceGroup*
	getFences();

	Window*
	getWindow();

	VulkanCommandBufferGroup*
	getCommandBuffers();

private:

	Window							*	m_window;

	VulkanInstance					*	m_instance;
	VulkanDevice					*	m_device;
	
	VulkanCommandBufferGroup		*	m_commandBufferGroup;
	VulkanFenceGroup				*	m_fences;

	VulkanCommandBuffer				*	m_setupCommandBuffer; //delete?
	uint32_t QUEUE_SLOT_COUNT = 2;								// delete?


	uint32_t							m_windowWidth, m_windowHeight;
	Game*			m_game;
	string			m_windowTitle;
	
	milliseconds	lastFrameTime;

	void
	init();

	void
	loop();
};



#endif