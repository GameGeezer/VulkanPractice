#pragma once

#include "BUILD_OPTIONS.h"
#include "Platform.h"

#include "Window.h"

#if VK_USE_PLATFORM_WIN32_KHR

enum KeyboardKeycode
{
	KEY_LEFT = VK_LEFT,
	KEY_RIGHT = VK_RIGHT,
	KEY_UP = VK_UP,
	KEY_DOWN = VK_DOWN,
	KEY_NUM_0 = 0x30,
	KEY_NUM_1 = 0x31,
	KEY_NUM_2 = 0x32,
	KEY_NUM_3 = 0x33,
	KEY_NUM_4 = 0x34,
	KEY_NUM_5 = 0x35,
	KEY_NUM_6 = 0x36,
	KEY_NUM_7 = 0x37,
	KEY_NUM_8 = 0x38,
	KEY_NUM_9 = 0x39
};

#endif