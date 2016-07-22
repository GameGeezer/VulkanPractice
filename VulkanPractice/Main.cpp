
#include "Application.h"
#include "Game.h"
#include "Screen.h"
#include "TestScreen.h"

int main()
{
	Screen *screen = new TestScreen();
	Game *game = new Game(*screen);
	Application *application = new Application(*game, "Test Screen", 640, 480);

	application->start();

	return 0;
}
/*
#include "VulkanQuad.h"
#include "Platform.h"

int main()
{

	AMD::VulkanSample* sample = nullptr;

	const auto sampleId = 0;

	switch (sampleId)
	{
	case 0: sample = new AMD::VulkanQuad; break;
	}

	if (sample == nullptr || sample->IsInitialized() == false)
	{
		return 1;
	}

	sample->Run(512);
	delete sample;

	return 0;
}
*/