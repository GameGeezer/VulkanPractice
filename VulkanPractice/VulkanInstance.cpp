#include "VulkanInstance.h"

#include <vector>

VulkanInstance::VulkanInstance()
{

}

VulkanInstance::~VulkanInstance()
{
	vkDestroyInstance(m_instance, nullptr);
}

bool
VulkanInstance::instantiate()
{
	VkInstanceCreateInfo instanceCreateInfo = {};
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

	instanceCreateInfo.ppEnabledExtensionNames = m_extensions.data();
	instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t> (m_extensions.size());
	instanceCreateInfo.ppEnabledLayerNames = m_layers.data();
	instanceCreateInfo.enabledLayerCount = static_cast<uint32_t> (m_layers.size());

	VkApplicationInfo applicationInfo = {};
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.apiVersion = VK_API_VERSION_1_0;
	applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	applicationInfo.pApplicationName = "AMD Vulkan Sample application";
	applicationInfo.pEngineName = "AMD Vulkan Sample Engine";

	instanceCreateInfo.pApplicationInfo = &applicationInfo;

	vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);

	return m_instance != VK_NULL_HANDLE;
}

void
VulkanInstance::addExtension(const char* extension)
{
	m_extensions.push_back(extension);
}

void
VulkanInstance::addLayer(const char* layer)
{
	m_layers.push_back(layer);
}

VkInstance
VulkanInstance::getHandle()
{
	return m_instance;
}