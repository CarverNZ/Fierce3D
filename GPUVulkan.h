#pragma once
#if defined(_WIN64)
#define VK_USE_PLATFORM_WIN32_KHR
#endif
#include <vulkan/vulkan.h>
#include "CF3DApp.h"
#include "CF3DBaseRenderer.h"

#if defined(_WIN64)
#include "PlatformWin64.h"
#endif


// Allow a maximum of two outstanding presentation operations.
constexpr uint32_t FRAME_LAG = 2;


class GPUVulkan : public Fierce3D::CF3DBaseRenderer
{
public:
	class PipelineBuilder {
	public:

		std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
		VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
		VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
		VkViewport _viewport;
		VkRect2D _scissor;
		VkPipelineRasterizationStateCreateInfo _rasterizer;
		VkPipelineColorBlendAttachmentState _colorBlendAttachment;
		VkPipelineMultisampleStateCreateInfo _multisampling;
		VkPipelineLayout _pipelineLayout;
		VkPipeline build_pipeline( VkDevice device, VkRenderPass pass );
	};

	GPUVulkan(Fierce3D::CF3DApp* app);
	~GPUVulkan();

	void InitializeGPU(Fierce3D::CF3DApp* _app) override;
	const std::string GetGPUDriverName();
	const std::string GetGPUVersion();
	void OnPostRender() override;
	void OnPreRender() override;
	void OnRender() override;
	void SetClearColor() override;
	void OnCleanup() override;

private:
	Fierce3D::CF3DApp* _app;
	bool			prepared = false;
	bool			use_staging_buffer = false;
	bool			separate_present_queue = false;
	bool			invalid_gpu_selection = false;
	int32_t			gpu_number = 0;
	uint32_t		current_buffer;
	uint32_t		queue_family_count;
	VkInstance		instance;
	VkSurfaceKHR	surface;
	VkDebugUtilsMessengerEXT debug_messenger;
	VkDevice		device;
	VkPhysicalDevice physicalDevice;
	VkQueue			graphicsQueue;
	VkQueue			presentQueue;
	VkRenderPass	renderPass;
	VkPipeline		graphicsPipeline;  // Holds the Vulkan graphics pipeline
	VkPipelineLayout pipelineLayout;
	VkFormat		SwapchainImageFormat;
	uint32_t		graphicsQueueFamilyIndex = 0;
	uint32_t		presentQueueFamilyIndex = 0;
	VkExtent2D		swapChainExtent;

	VkPhysicalDeviceProperties gpu_props;
	std::vector<VkQueueFamilyProperties> queue_props;
	VkPhysicalDeviceMemoryProperties memory_properties;
	VkSwapchainKHR swapChain;
	std::vector<const char*> enabled_instance_extensions;
	std::vector<const char*> enabled_layers;
	std::vector<const char*> enabled_device_extensions;
	std::vector<VkImageView> swapchainImageViews;

	void CreateSurface();
	void CreateLogicalDevice();
	VkSwapchainKHR CreateSwapChain();
	void CreateImageViews();
	void CreateRenderPass();
	void CreateFrameBuffers();
	void CreateRenderPipeline();
	void CreatePipelineLayout();
	void CreateGraphicsPipeline();
	VkShaderModule CreateShaderModule(VkDevice device, const std::string& filepath);
	VkPhysicalDevice ChooseBestPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
};
