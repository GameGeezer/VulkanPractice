#include "Renderer.h"

#include "VulkanHelper.h"

Renderer::Renderer()
{
	_initInstance();

	_initDevices();
}

Renderer::~Renderer()
{

}

VulkanDevice* Renderer::getDevice(int number)
{
	return (*vulkanDevices)[number];
}

void Renderer::_initInstance()
{
	VkApplicationInfo applicationInfo { };
	// Filling out application description:
	// sType is mandatory
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	// pNext is mandatory
	applicationInfo.pNext = NULL;
	// The name of our application
	applicationInfo.pApplicationName = "Tutorial 1";
	// The name of the engine (e.g: Game engine name)
	applicationInfo.pEngineName = NULL;
	// The version of the engine
	applicationInfo.engineVersion = 1;
	// The version of Vulkan we're using for this application
	applicationInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo instanceInfo { };
	// Filling out instance description:
	// sType is mandatory
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	// pNext is mandatory
	instanceInfo.pNext = NULL;
	// flags is mandatory
	instanceInfo.flags = 0;
	// The application info structure is then passed through the instance
	instanceInfo.pApplicationInfo = &applicationInfo;
	// Don't enable and layer
	instanceInfo.enabledLayerCount = 0;
	instanceInfo.ppEnabledLayerNames = NULL;
	// Don't enable any extensions
	instanceInfo.enabledExtensionCount = 0;
	instanceInfo.ppEnabledExtensionNames = NULL;

	// Now create the desired instance
	assertVulkan(vkCreateInstance(&instanceInfo, NULL, &instance));
}

void Renderer::_initDevices()
{
	// Query how many devices are present in the system
	assertVulkan(vkEnumeratePhysicalDevices(instance, &deviceCount, NULL));

	// There has to be at least one device present
	if (deviceCount == 0) {

		exit(EXIT_FAILURE);
	}
	vulkanDevices = new vector<VulkanDevice*>(deviceCount);
	// Get the physical devices
	vector<VkPhysicalDevice> physicalDevices(deviceCount);
	assertVulkan(vkEnumeratePhysicalDevices(instance, &deviceCount, &(physicalDevices[0])));

	vector<uint32_t> queueFamilyCount(deviceCount);
	vector<VkQueueFamilyProperties> queueFamilyProperties(deviceCount);

	vector<VkDevice> devices(deviceCount);
	vector<VkPhysicalDeviceProperties> physicalDeviceProperties(deviceCount);

	for (uint32_t i = 0; i < deviceCount; i++) {
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &(queueFamilyCount[i]), NULL);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevices[i], &(queueFamilyCount[i]), &(queueFamilyProperties[i]));

		_createDevice(physicalDevices[i], devices[i]);
		_createDeviceProperties(physicalDevices[i], physicalDeviceProperties[i]);

		(*vulkanDevices)[i] = new VulkanDevice(devices[i], physicalDevices[i], physicalDeviceProperties[i], queueFamilyCount[i], queueFamilyProperties[i]);
	}
}

void Renderer::_createDevice(VkPhysicalDevice& physicalDevice, VkDevice& outDevice)
{
	VkDeviceCreateInfo deviceInfo { };
	// Mandatory fields
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.pNext = NULL;
	deviceInfo.flags = 0;

	// We won't bother with extensions or layers
	deviceInfo.enabledLayerCount = 0;
	deviceInfo.ppEnabledLayerNames = NULL;
	deviceInfo.enabledExtensionCount = 0;
	deviceInfo.ppEnabledExtensionNames = NULL;

	// We don't want any any features,:the wording in the spec for DeviceCreateInfo
	// excludes that you can pass NULL to disable features, which GetPhysicalDeviceFeatures
	// tells you is a valid value. This must be supplied - NULL is legal here.
	deviceInfo.pEnabledFeatures = NULL;

	// Here's where we initialize our queues
	VkDeviceQueueCreateInfo deviceQueueInfo { };
	deviceQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceQueueInfo.pNext = NULL;
	deviceQueueInfo.flags = 0;
	// Use the first queue family in the family list
	deviceQueueInfo.queueFamilyIndex = 0;

	// Create only one queue
	float queuePriorities[] = { 1.0f };
	deviceQueueInfo.queueCount = 1;
	deviceQueueInfo.pQueuePriorities = queuePriorities;
	// Set queue(s) into the device
	deviceInfo.queueCreateInfoCount = 1;
	deviceInfo.pQueueCreateInfos = &deviceQueueInfo;

	assertVulkan(vkCreateDevice(physicalDevice, &deviceInfo, NULL, &outDevice));
}

void Renderer::_createDeviceProperties(VkPhysicalDevice& device, VkPhysicalDeviceProperties& outProperties)
{
	memset(&outProperties, 0, sizeof outProperties);
	vkGetPhysicalDeviceProperties(device, &outProperties);
	printf("Driver Version: %d\n", outProperties.driverVersion);
	printf("Device Name:    %s\n", outProperties.deviceName);
	printf("Device Type:    %d\n", outProperties.deviceType);
	printf("API Version:    %d.%d.%d\n",
		// See note below regarding this:
		(outProperties.apiVersion >> 22) & 0x3FF,
		(outProperties.apiVersion >> 12) & 0x3FF,
		(outProperties.apiVersion & 0xFFF));
}

void Renderer::_dubugFamilyQueueCheckSupported(VkQueueFamilyProperties& familitProperties)
{
	printf("Count of Queues: %d\n", familitProperties.queueCount);
	printf("Supported operationg on this queue:\n");
	if (familitProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		printf("\t\t Graphics\n");
	if (familitProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)
		printf("\t\t Compute\n");
	if (familitProperties.queueFlags & VK_QUEUE_TRANSFER_BIT)
		printf("\t\t Transfer\n");
	if (familitProperties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)
		printf("\t\t Sparse Binding\n");
}