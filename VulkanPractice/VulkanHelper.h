#pragma once

#include <vulkan\vulkan.h>
#include <stdio.h>
#include <stdlib.h>

#define assertVulkan(ans) { checkVulkanForError(ans, __FILE__, __LINE__); }
inline void checkVulkanForError(VkResult result , const char *file, int line, bool abort = true)
{
	if (result != VK_SUCCESS)
	{
		fprintf(stderr, "Failed to query the number of physical devices present: %d\n", result);

		if (abort)
		{
			exit(EXIT_FAILURE);
		}
	}
}