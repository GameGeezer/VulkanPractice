/* -----------------------------------------------------
This source code is public domain ( CC0 )
The code is provided as-is without limitations, requirements and responsibilities.
Creators and contributors to this source code are provided as a token of appreciation
and no one associated with this source code can be held responsible for any possible
damages or losses of any kind.
Original file creator:  Niko Kauppi (Code maintenance)
Contributors:
----------------------------------------------------- */

#include "BUILD_OPTIONS.h"
#include "Platform.h"

#include "Renderer.h"
#include "Shared.h"
#include "Window.h"

#include <cstdlib>
#include <assert.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <array>

Renderer::Renderer()
{
	InitPlatform();
	setupLayersAndExtensions();
	setupDebug();
	initInstance();
	initDebug();
	initDevice();

	initRenderPass();
}

Renderer::~Renderer()
{
	deInitDevice();
	deInitDebug();
	deInitInstance();
	DeInitPlatform();
}

const VkInstance Renderer::GetVulkanInstance() const
{
	return m_instance;
}

const VkPhysicalDevice Renderer::GetVulkanPhysicalDevice() const
{
	return m_gpu;
}

const VkDevice Renderer::GetVulkanDevice() const
{
	return m_device;
}

const VkQueue Renderer::GetVulkanQueue() const
{
	return m_queue;
}

const uint32_t Renderer::GetVulkanGraphicsQueueFamilyIndex() const
{
	return m_graphicsFamilyIndex;
}

const VkPhysicalDeviceProperties & Renderer::GetVulkanPhysicalDeviceProperties() const
{
	return m_gpuProperties;
}

void Renderer::setupLayersAndExtensions()
{
	// Add surfaces as an extension
	instanceExtensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
	// Add the extensions specific to the platform
	AddRequiredPlatformInstanceExtensions(&instanceExtensions);
	// Add swap chaings to our extensions
	deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

void Renderer::initRenderPass()
{
	// Render passes are a new concept in Vulkan
	// They describe the attachments used during rendering
	// and may contain multiple subpasses with attachment
	// dependencies 
	// This allows the driver to know up-front how the
	// rendering will look like and is a good opportunity to
	// optimize, especially on tile-based renderers (with multiple subpasses)

	// This example will use a single render pass with
	// one subpass, which is the minimum setup

	// Two attachments
	// Basic setup with a color and a depth attachments
	std::array<VkAttachmentDescription, 2> attachments = {};

	// Color attachment
	attachments[0].format = VK_FORMAT_B8G8R8A8_UNORM;
	// We don't use multi sampling
	// Multi sampling requires a setup with resolve attachments
	// See the multisampling example for more details
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	// loadOp describes what happens with the attachment content at the beginning of the subpass
	// We want the color buffer to be cleared
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	// storeOp describes what happens with the attachment content after the subpass is finished
	// As we want to display the color attachment after rendering is done we have to store it
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	// We don't use stencil and DONT_CARE allows the driver to discard the result 
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	// Set the initial image layout for the color atttachment
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// Depth attachment
	attachments[1].format = depthFormat;
	attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
	// Clear depth at the beginnging of the subpass
	attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	// We don't need the contents of the depth buffer after the sub pass
	// anymore, so let the driver discard it
	attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	// Don't care for stencil 
	attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	// Set the initial image layout for the depth attachment
	attachments[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// Setup references to our attachments for the sub pass
	VkAttachmentReference colorReference = {};
	colorReference.attachment = 0;
	colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthReference = {};
	depthReference.attachment = 1;
	depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	// Setup a single subpass that references our color and depth attachments
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	// Input attachments can be used to sample from contents of attachments 
	// written to by previous sub passes in a setup with multiple sub passes
	// This example doesn't make use of this
	subpass.inputAttachmentCount = 0;
	subpass.pInputAttachments = nullptr;
	// Preserved attachments can be used to loop (and preserve) attachments
	// through a sub pass that does not actually use them
	// This example doesn't make use of this
	subpass.preserveAttachmentCount = 0;
	subpass.pPreserveAttachments = nullptr;
	// Resoluve attachments are resolved at the end of a sub pass and can be
	// used for e.g. multi sampling
	// This example doesn't make use of this
	subpass.pResolveAttachments = nullptr;
	// Reference to the color attachment
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorReference;
	// Reference to the depth attachment
	subpass.pDepthStencilAttachment = &depthReference;

	// Setup the render pass
	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	// Set attachments used in our renderpass
	renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments = attachments.data();
	// We only use one subpass
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	// As we only use one subpass we don't have any subpass dependencies
	renderPassInfo.dependencyCount = 0;
	renderPassInfo.pDependencies = nullptr;

	// Create the renderpass
	ErrorCheck(vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass));
}

void Renderer::initDescriptorSetLayout()
{
	// Setup layout of descriptors used in this example
	// Basically connects the different shader stages to descriptors
	// for binding uniform buffers, image samplers, etc.
	// So every shader binding should map to one descriptor set layout
	// binding

	// Binding 0 : Uniform buffer (Vertex shader)
	VkDescriptorSetLayoutBinding layoutBinding = {};
	layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	layoutBinding.descriptorCount = 1;
	layoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	layoutBinding.pImmutableSamplers = NULL;

	VkDescriptorSetLayoutCreateInfo descriptorLayout = {};
	descriptorLayout.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorLayout.pNext = NULL;
	descriptorLayout.bindingCount = 1;
	descriptorLayout.pBindings = &layoutBinding;

	ErrorCheck(vkCreateDescriptorSetLayout(m_device, &descriptorLayout, NULL, &m_descriptorSetLayout));

	// Create the pipeline layout that is used to generate the rendering pipelines that
	// are based on this descriptor set layout
	// In a more complex scenario you would have different pipeline layouts for different
	// descriptor set layouts that could be reused
	VkPipelineLayoutCreateInfo pPipelineLayoutCreateInfo = {};
	pPipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pPipelineLayoutCreateInfo.pNext = NULL;
	pPipelineLayoutCreateInfo.setLayoutCount = 1;
	pPipelineLayoutCreateInfo.pSetLayouts = &m_descriptorSetLayout;

	ErrorCheck(vkCreatePipelineLayout(m_device, &pPipelineLayoutCreateInfo, nullptr, &m_pipelineLayout));
}

void Renderer::initInstance()
{
	VkApplicationInfo application_info{};
	application_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	application_info.apiVersion = VK_MAKE_VERSION(1, 0, 2);			// 1.0.2 should work on all vulkan enabled drivers.
	application_info.applicationVersion = VK_MAKE_VERSION(0, 1, 0);
	application_info.pApplicationName = "Vulkan API Tutorial Series";

	VkInstanceCreateInfo instance_create_info{};
	instance_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_create_info.pApplicationInfo = &application_info;
	instance_create_info.enabledLayerCount = instanceLayers.size();
	instance_create_info.ppEnabledLayerNames = instanceLayers.data();
	instance_create_info.enabledExtensionCount = instanceExtensions.size();		// The number of extensions being added
	instance_create_info.ppEnabledExtensionNames = instanceExtensions.data();	// A list of the extensions
	instance_create_info.pNext = &_debug_callback_create_info;

	ErrorCheck(vkCreateInstance(&instance_create_info, nullptr, &m_instance));
}

void Renderer::deInitInstance()
{
	vkDestroyInstance(m_instance, nullptr);
	m_instance = nullptr;
}

void Renderer::initDevice()
{
	{
		uint32_t gpu_count = 0;
		vkEnumeratePhysicalDevices(m_instance, &gpu_count, nullptr);
		std::vector<VkPhysicalDevice> gpu_list(gpu_count);
		vkEnumeratePhysicalDevices(m_instance, &gpu_count, gpu_list.data());
		m_gpu = gpu_list[0];
		vkGetPhysicalDeviceProperties(m_gpu, &m_gpuProperties);
	}
	{
		uint32_t family_count = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(m_gpu, &family_count, nullptr);
		std::vector<VkQueueFamilyProperties> family_property_list(family_count);
		vkGetPhysicalDeviceQueueFamilyProperties(m_gpu, &family_count, family_property_list.data());

		bool found = false;
		for (uint32_t i = 0; i < family_count; ++i) {
			if (family_property_list[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				found = true;
				m_graphicsFamilyIndex = i;
			}
		}
		if (!found) {
			assert(0 && "Vulkan ERROR: Queue family supporting graphics not found.");
			std::exit(-1);
		}
	}

	float queue_priorities[]{ 1.0f };
	VkDeviceQueueCreateInfo device_queue_create_info{};
	device_queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	device_queue_create_info.queueFamilyIndex = m_graphicsFamilyIndex;
	device_queue_create_info.queueCount = 1;
	device_queue_create_info.pQueuePriorities = queue_priorities;

	VkDeviceCreateInfo device_create_info{};
	device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_create_info.queueCreateInfoCount = 1;
	device_create_info.pQueueCreateInfos = &device_queue_create_info;
	//	device_create_info.enabledLayerCount		= _device_layers.size();			// depricated
	//	device_create_info.ppEnabledLayerNames		= _device_layers.data();			// depricated
	device_create_info.enabledExtensionCount = deviceExtensions.size();
	device_create_info.ppEnabledExtensionNames = deviceExtensions.data();

	ErrorCheck(vkCreateDevice(m_gpu, &device_create_info, nullptr, &m_device));

	vkGetDeviceQueue(m_device, m_graphicsFamilyIndex, 0, &m_queue);


	//VkBool32 validDepthFormat = vkTools::getSupportedDepthFormat(m_gpu, &depthFormat);
	//assert(validDepthFormat);
}

void Renderer::deInitDevice()
{
	vkDestroyDevice(m_device, nullptr);
	m_device = nullptr;
}

#if BUILD_ENABLE_VULKAN_DEBUG

VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanDebugCallback(
	VkDebugReportFlagsEXT		flags,
	VkDebugReportObjectTypeEXT	obj_type,
	uint64_t					src_obj,
	size_t						location,
	int32_t						msg_code,
	const char *				layer_prefix,
	const char *				msg,
	void *						user_data
	)
{
	std::ostringstream stream;
	stream << "VKDBG: ";
	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		stream << "INFO: ";
	}
	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		stream << "WARNING: ";
	}
	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		stream << "PERFORMANCE: ";
	}
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		stream << "ERROR: ";
	}
	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		stream << "DEBUG: ";
	}
	stream << "@[" << layer_prefix << "]: ";
	stream << msg << std::endl;
	std::cout << stream.str();

#if defined( _WIN32 )
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		MessageBox(NULL, stream.str().c_str(), "Vulkan Error!", 0);
	}
#endif

	return false;
}

void Renderer::setupDebug()
{
	_debug_callback_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	_debug_callback_create_info.pfnCallback = VulkanDebugCallback;
	_debug_callback_create_info.flags =
		//		VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
		VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT |
		//		VK_DEBUG_REPORT_DEBUG_BIT_EXT |
		0;

	instanceLayers.push_back("VK_LAYER_LUNARG_standard_validation");
	/*
	//	_instance_layers.push_back( "VK_LAYER_LUNARG_threading" );
	_instance_layers.push_back( "VK_LAYER_GOOGLE_threading" );
	_instance_layers.push_back( "VK_LAYER_LUNARG_draw_state" );
	_instance_layers.push_back( "VK_LAYER_LUNARG_image" );
	_instance_layers.push_back( "VK_LAYER_LUNARG_mem_tracker" );
	_instance_layers.push_back( "VK_LAYER_LUNARG_object_tracker" );
	_instance_layers.push_back( "VK_LAYER_LUNARG_param_checker" );
	*/
	instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);

	//	_device_layers.push_back( "VK_LAYER_LUNARG_standard_validation" );			// depricated
	/*
	//	_device_layers.push_back( "VK_LAYER_LUNARG_threading" );
	_device_layers.push_back( "VK_LAYER_GOOGLE_threading" );
	_device_layers.push_back( "VK_LAYER_LUNARG_draw_state" );
	_device_layers.push_back( "VK_LAYER_LUNARG_image" );
	_device_layers.push_back( "VK_LAYER_LUNARG_mem_tracker" );
	_device_layers.push_back( "VK_LAYER_LUNARG_object_tracker" );
	_device_layers.push_back( "VK_LAYER_LUNARG_param_checker" );
	*/
}

PFN_vkCreateDebugReportCallbackEXT		fvkCreateDebugReportCallbackEXT = nullptr;
PFN_vkDestroyDebugReportCallbackEXT		fvkDestroyDebugReportCallbackEXT = nullptr;

void Renderer::initDebug()
{
	fvkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkCreateDebugReportCallbackEXT");
	fvkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT");
	if (nullptr == fvkCreateDebugReportCallbackEXT || nullptr == fvkDestroyDebugReportCallbackEXT) {
		assert(0 && "Vulkan ERROR: Can't fetch debug function pointers.");
		std::exit(-1);
	}

	fvkCreateDebugReportCallbackEXT(m_instance, &_debug_callback_create_info, nullptr, &_debug_report);

	//	vkCreateDebugReportCallbackEXT( _instance, nullptr, nullptr, nullptr );
}

void Renderer::deInitDebug()
{
	fvkDestroyDebugReportCallbackEXT(m_instance, _debug_report, nullptr);
	_debug_report = VK_NULL_HANDLE;
}

#else

void Renderer::_SetupDebug() {};
void Renderer::_InitDebug() {};
void Renderer::_DeInitDebug() {};

#endif // BUILD_ENABLE_VULKAN_DEBUG