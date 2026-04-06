
#include "Fierce3D.h"
#include "GPUVulkan.h"
#include "CF3DApp.h"


#if defined(VK_USE_PLATFORM_XLIB_KHR) || defined(VK_USE_PLATFORM_XCB_KHR)
#include <X11/Xutil.h>
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
#include <linux/input.h>
#include "xdg-shell-client-header.h"
#include "xdg-decoration-client-header.h"
#endif

#include <optional>
#include <vector>
#include <stdexcept>
#include <algorithm>  // Required for std::max and std::min
#include <cstdlib>
#include <istream>
#include <iostream>
#include <fstream>
#pragma comment(lib,"vulkan-1.lib" )


GPUVulkan::GPUVulkan(Fierce3D::CF3DApp* app) : _app(app), instance(VK_NULL_HANDLE), device(VK_NULL_HANDLE), swapChain(VK_NULL_HANDLE),
surface(VK_NULL_HANDLE), physicalDevice(VK_NULL_HANDLE), graphicsQueue(VK_NULL_HANDLE),
presentQueue(VK_NULL_HANDLE), debug_messenger(VK_NULL_HANDLE)
{
	if (_app == nullptr)
		throw std::invalid_argument("Invalid Fierce3D Application call to Vulkan");
	_app = app;

}


GPUVulkan::~GPUVulkan()
{

}


bool isDeviceSuitable(VkPhysicalDevice device)
{
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
	return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU && deviceFeatures.geometryShader;
}


uint32_t FindGraphicsQueueFamily(VkPhysicalDevice device)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	for (uint32_t i = 0; i < queueFamilyCount; i++)
	{
		if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			return i;
		}
	}
	throw std::runtime_error("Failed to find a suitable queue family!");
}


std::vector<const char*> getRequiredExtensions()
{
	// Example: List of required extensions for a basic setup (add more if needed)
	std::vector<const char*> extensions;
	// VK_KHR_surface is required to create surfaces on different platforms
	extensions.push_back("VK_KHR_surface");
	// Enable platform-specific extensions
#if defined(_WIN32)
	extensions.push_back("VK_KHR_win32_surface");
#elif defined(__linux__)
	extensions.push_back("VK_KHR_xcb_surface");
#elif defined(__APPLE__)
	extensions.push_back("VK_EXT_metal_surface");
#endif
	// Add any other extensions required by your application
	return extensions;
}


bool checkInstanceExtensionSupport(const std::vector<const char*>& requiredExtensions)
{
	uint32_t extensionCount = 0;
	vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, nullptr );
	std::vector<VkExtensionProperties> availableExtensions( extensionCount );
	vkEnumerateInstanceExtensionProperties( nullptr, &extensionCount, availableExtensions.data() );
	// Check for the presence of each required extension
	for ( const char* extension : requiredExtensions )
	{
		bool extensionFound = false;
		for ( const auto& ext : availableExtensions )
		{
			if ( strcmp( extension, ext.extensionName ) == 0 )
			{
				extensionFound = true;
				break;
			}
		}
		if ( !extensionFound )
		{
			std::cerr << "Extension not found: " << extension << std::endl;
			return false;
		}
	}
	return true;
}


struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;
	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};


// Function to find suitable queue family indices
QueueFamilyIndices FindQueueFamilies( VkPhysicalDevice device, VkSurfaceKHR surface )
{
	QueueFamilyIndices indices;
	// Get the number of queue families supported by the physical device
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, nullptr );
	// Retrieve the queue family properties
	std::vector<VkQueueFamilyProperties> queueFamilies( queueFamilyCount );
	vkGetPhysicalDeviceQueueFamilyProperties( device, &queueFamilyCount, queueFamilies.data() );
	// Iterate over each queue family and check for required capabilities
	int i = 0;
	for ( const auto& queueFamily : queueFamilies )
	{
		// Check if this queue family supports graphics commands
		if ( queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT )
		{
			indices.graphicsFamily = i;
		}
		// Check if this queue family supports presentation to the given surface
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR( device, i, surface, &presentSupport );
		if ( presentSupport )
		{
			indices.presentFamily = i;
		}
		// If both indices are found, break early
		if ( indices.isComplete() )
		{
			break;
		}
		i++;
	}
	return indices;
}



void GPUVulkan::InitializeGPU(Fierce3D::CF3DApp* _app)
{
	// Optional: Specify application info (used for optimizations)
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Vulkan App";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Fierce3D Engine";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_2;

	// Specify instance creation information
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// Enable required extensions
// Step 1: Get the list of required extensions
	std::vector<const char*> requiredExtensions = getRequiredExtensions();
	createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
	createInfo.ppEnabledExtensionNames = requiredExtensions.data();
	// Optional: Enable validation layers (debugging)
	const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	createInfo.ppEnabledLayerNames = validationLayers.data();
	// Create Vulkan instance
	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Vulkan instance");
	}
	// Create Surface
	CreateSurface();
	// Pick a Physical Device
	physicalDevice = ChooseBestPhysicalDevice(instance, surface);
	CreateLogicalDevice();
	swapChain = CreateSwapChain();
	CreateImageViews();
	CreateRenderPass();
	CreateFrameBuffers();
	CreatePipelineLayout();
	CreateRenderPipeline();
}


VkPhysicalDevice GPUVulkan::ChooseBestPhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
	int bestScore = 0;

	for (const auto& device : devices)
	{
		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(device, &deviceProperties);
		vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

		// Assign a score to the device based on certain criteria
		int score = 0;
		if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			score += 1000;  // Prefer discrete GPUs
		}

		// Check if the device supports required extensions and features
		if (deviceFeatures.geometryShader)
		{
			score += 100;
		}

		// Check queue families for graphics and presentation support
		QueueFamilyIndices indices = FindQueueFamilies(device, surface);
		if (indices.isComplete())
		{
			score += 500;
		}

		// If this device has a better score, choose it
		if (score > bestScore)
		{
			bestScore = score;
			bestDevice = device;
		}
	}

	if (bestDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("Failed to find a suitable GPU!");
	}
	return bestDevice;
}



void GPUVulkan::CreateSurface()
{
	//	void createWin32Surface(VkInstance instance, HWND hwnd, HINSTANCE hInstance) {
	VkWin32SurfaceCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	createInfo.pNext = nullptr;
#if defined(_WIN64)
	createInfo.hwnd = GetWindowHandle();          // Handle to the Win32 window
	createInfo.hinstance = GetInstance();		  // hInstance;  // Application handle
#endif
	if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Win32 Vulkan surface!");
	}
}

void GPUVulkan::CreateLogicalDevice()
{
	uint32_t graphicsQueueFamily = FindGraphicsQueueFamily(physicalDevice);
	if (graphicsQueueFamily == UINT32_MAX)
	{
		throw std::runtime_error("Failed to find a valid graphics queue family!");
	}

	// Queue creation
	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = graphicsQueueFamily;
	queueCreateInfo.queueCount = 1;
	float queuePriority = 1.0f;
	queueCreateInfo.pQueuePriorities = &queuePriority;

	// Required device features
	VkPhysicalDeviceFeatures deviceFeatures{};
	deviceFeatures.samplerAnisotropy = VK_TRUE;  // Example feature

	// Device extensions (e.g., for swapchain support)
	std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	// Logical device creation info
	VkDeviceCreateInfo vulkanCreateInfo{};
	vulkanCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	vulkanCreateInfo.queueCreateInfoCount = 1;
	vulkanCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	vulkanCreateInfo.pEnabledFeatures = &deviceFeatures;
	vulkanCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	vulkanCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();

	// Enable validation layers if debugging
#ifdef ENABLE_VALIDATION_LAYERS
	std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
	vulkanCreateInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
	vulkanCreateInfo.ppEnabledLayerNames = validationLayers.data();
#else
	vulkanCreateInfo.enabledLayerCount = 0;
#endif

	// Create the logical device
	VkResult result = vkCreateDevice(physicalDevice, &vulkanCreateInfo, nullptr, &device);
	if (result != VK_SUCCESS)
	{
		std::cerr << "Failed to create logical device. Error code: " << result << std::endl;
		throw std::runtime_error("Failed to create logical device!");
	}

	// Retrieve the graphics queue
	vkGetDeviceQueue(device, graphicsQueueFamily, 0, &graphicsQueue);
}


// Structure to hold details about swapchain support
struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};


// Function to query swapchain support details for a given surface and physical device
SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
	SwapChainSupportDetails details;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}
	return details;
}


// Choose a suitable surface format (color depth)
VkSurfaceFormatKHR ChooseSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& format : availableFormats)
	{
		if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return format;
		}
	}
	return availableFormats[0];
}


// Choose a suitable present mode (vsync, immediate, etc.)
VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& presentMode : availablePresentModes)
	{
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return presentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;  // Guaranteed to be available
}


// Determine the swap extent (resolution) of the swapchain images
VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, uint32_t width, uint32_t height)
{
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		VkExtent2D actualExtent = { width, height };
		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return actualExtent;
	}
}


VkSwapchainKHR GPUVulkan::CreateSwapChain()
{
	// Query swapchain support details
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice, surface);
	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice, surface);

	// Retrieve queue family properties
	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queue_family_count, nullptr);
	std::vector<VkQueueFamilyProperties> queue_props(queue_family_count);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queue_family_count, queue_props.data());

	// Get present support for each queue
	std::vector<VkBool32> supportsPresent(queue_family_count);
	for (uint32_t i = 0; i < queue_family_count; i++)
	{
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &supportsPresent[i]);
	}

	// Select queue family indices
	graphicsQueueFamilyIndex = indices.graphicsFamily.value_or(UINT32_MAX);
	presentQueueFamilyIndex = indices.presentFamily.value_or(UINT32_MAX);

	if (graphicsQueueFamilyIndex == UINT32_MAX || presentQueueFamilyIndex == UINT32_MAX)
	{
		throw std::runtime_error("Failed to find required queue families!");
	}

	// Choose surface format, present mode, and swap extent
	VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChoosePresentMode(swapChainSupport.presentModes);
	swapChainExtent = ChooseSwapExtent(swapChainSupport.capabilities, _app->GetVideoCreateInfo()->width, _app->GetVideoCreateInfo()->height);

	// Check image count
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	// Set up swapchain creation info
	SwapchainImageFormat = surfaceFormat.format;
	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = SwapchainImageFormat;//  surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = swapChainExtent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	if (graphicsQueueFamilyIndex != presentQueueFamilyIndex)
	{
		uint32_t queueFamilyIndices[] = { graphicsQueueFamilyIndex, presentQueueFamilyIndex };
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;
	createInfo.oldSwapchain = VK_NULL_HANDLE;  // For initial swapchain creation

	std::cout << "Creating swapchain with image count: " << imageCount << std::endl;
	std::cout << "Using surface format: " << surfaceFormat.format << std::endl;
	std::cout << "Present mode: " << presentMode << std::endl;
	std::cout << "Extent: " << swapChainExtent.width << " x " << swapChainExtent.height << std::endl;
	if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty())
	{
		throw std::runtime_error("No suitable surface formats or present modes found!");
	}
	// Create swapchain and validate result
	VkSwapchainKHR newSwapchain;
	VkResult result = vkCreateSwapchainKHR(device, &createInfo, nullptr, &newSwapchain);
	if (result != VK_SUCCESS)
	{
		std::cerr << "Failed to create swapchain. VkResult: " << result << std::endl;
		throw std::runtime_error("Failed to create swapchain!");
	}
	return newSwapchain;
}


void GPUVulkan::CreateImageViews()
{
	uint32_t imageCount;
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
	std::vector<VkImage> swapchainImages(imageCount);
	vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapchainImages.data());
	//std::vector<VkImageView> swapchainImageViews(imageCount);
	swapchainImageViews.resize(imageCount);
	for (size_t i = 0; i < imageCount; i++)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = swapchainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = VK_FORMAT_B8G8R8A8_SRGB;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;
		if (vkCreateImageView(device, &createInfo, nullptr, &swapchainImageViews[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create image views!");
		}
	}
}


void GPUVulkan::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = SwapchainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;
	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	if (vkCreateRenderPass( device, &renderPassInfo, nullptr, &renderPass ) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create render pass!");
	}
}


void GPUVulkan::CreateFrameBuffers()
{
	std::vector<VkFramebuffer> swapChainFramebuffers( swapchainImageViews.size() );
	for (size_t i = 0; i < swapchainImageViews.size(); i++)
	{
		VkImageView attachments[] = { swapchainImageViews[i] };
		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width =  swapChainExtent.width;
		framebufferInfo.height = swapChainExtent.height;
		framebufferInfo.layers = 1;
		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create framebuffer!");
		}
	}
}



void GPUVulkan::CreatePipelineLayout()
{
	VkDescriptorSetLayout descriptorSetLayout;
	// Create a descriptor set layout (example with a single uniform buffer binding)
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;  // Optional
	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;
	if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create descriptor set layout!");
	}
	// Define the pipeline layout create info structure
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;  // Number of descriptor sets
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;  // Pointer to the descriptor set layout
	pipelineLayoutInfo.pushConstantRangeCount = 0;  // Optional: Define any push constant ranges

	// Create the pipeline layout
	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create pipeline layout!");
	}
}


void GPUVulkan::CreateRenderPipeline()
{
	VkShaderModule vertShaderModule = CreateShaderModule(device, "path/to/vertex_shader.spv");
	VkShaderModule fragShaderModule = CreateShaderModule(device, "path/to/fragment_shader.spv");

	
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapChainExtent.width;
	viewport.height = (float)swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChainExtent;
	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;
	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	// Add more configurations like multisampling, color blending, etc.
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
//	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create graphics pipeline!");
	}
}


void GPUVulkan::CreateGraphicsPipeline()
{
	// Load shaders
	VkShaderModule vertShaderModule = CreateShaderModule(device, "path/to/vertex_shader.spv");
	VkShaderModule fragShaderModule = CreateShaderModule(device, "path/to/fragment_shader.spv");
	// Setup shader stage info
	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main"; // Entry point name
	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main"; // Entry point name
	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };
	// Create graphics pipeline
	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	// Other pipeline settings...
	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create graphics pipeline!");
	}
	// Cleanup shader modules
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
}


VkShaderModule GPUVulkan::CreateShaderModule(VkDevice device, const std::string& filepath)
{
	// Read the shader code from the file
	std::ifstream file(filepath, std::ios::ate | std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open shader file: " + filepath);
	}
	// Get the file size
	size_t fileSize = (size_t)file.tellg();
	file.seekg(0);
	// Read the file contents into a vector
	std::vector<char> buffer(fileSize);
	file.read(buffer.data(), fileSize);
	file.close();
	// Create the shader module
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = buffer.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(buffer.data()); // Cast to uint32_t*

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create shader module!");
	}

	return shaderModule;
}


void GPUVulkan::OnPostRender()
{
}

void GPUVulkan::OnPreRender()
{
}

void GPUVulkan::OnRender()
{
}

void GPUVulkan::SetClearColor() {}


void GPUVulkan::OnCleanup()
{
	/*	Vulkan objects must be destroyed in the reverse order of their creation:
		Destroy the swapchain before destroying the logical device.
		Destroy the surface after the swapchain.
		Destroy the Vulkan instance last.
	*/
	/*	vkDestroySemaphore(device, renderFinishedSemaphore, nullptr);
		vkDestroySemaphore(device, imageAvailableSemaphore, nullptr);
		vkDestroyCommandPool(device, commandPool, nullptr);
		for (auto imageView : swapChainImageViews)
		{
			vkDestroyImageView(device, imageView, nullptr);
		} */
	vkDestroySwapchainKHR(device, swapChain, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroyInstance(instance, nullptr);
}


const std::string GPUVulkan::GetGPUDriverName()
{
	return std::string("Vulkan");
}


const std::string GPUVulkan::GetGPUVersion()
{
	return std::string("1.0");
}
