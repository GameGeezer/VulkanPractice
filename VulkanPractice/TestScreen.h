#pragma once

#include "Screen.h"

class TestScreen : public Screen
{
	void
		onCreate() override;

	void
		onPause() override;

	void
		onLeave() override;

	void
		onResume() override;

	void
		onUpdate(int delta) override;

	void
		onResize(int width, int height) override;

	void
		onDestroy() override;
};
