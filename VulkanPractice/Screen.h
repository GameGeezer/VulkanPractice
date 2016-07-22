#ifndef VALIS_SCREEN_H
#define VALIS_SCREEN_H

#include <stdint.h>

class Screen
{
	friend class Game;
protected:

	virtual void
		onCreate() = 0;

	virtual void
		onPause() = 0;

	virtual void
		onLeave() = 0;

	virtual void
		onResume() = 0;

	virtual void
		onUpdate(uint32_t delta) = 0;

	virtual void
		onResize(uint32_t width, uint32_t height) = 0;

	virtual void
		onDestroy() = 0;

private:
	bool isCreated;
};

#endif