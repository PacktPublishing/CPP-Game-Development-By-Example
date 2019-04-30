#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#include <vulkan\vulkan.h>

#include "AppValidationLayersAndExtensions.h"
#include "VulkanInstance.h"
#include "Device.h"

#include "SwapChain.h"
#include "Renderpass.h"
#include "RenderTarget.h"
#include "DrawCommandBuffer.h"

#ifdef _DEBUG
const bool isValidationLayersEnabled = true;
#else
const bool isValidationLayersEnabled = false;
#endif

class VulkanContext {

public:
	
	static VulkanContext* getInstance();
	static VulkanContext* instance;

	~VulkanContext();

	void initVulkan(GLFWwindow* window);

	void drawBegin();
	void drawEnd();
	void cleanup();

	Device* getDevice();

private:

	uint32_t imageIndex = 0;
	VkCommandBuffer curentCommandBuffer;

	//surface
	VkSurfaceKHR surface;

	// My Classes
	AppValidationLayersAndExtensions *valLayersAndExt;
	VulkanInstance* vInstance;
	Device* device;

	SwapChain* swapChain;
	Renderpass* renderPass;
	RenderTarget* renderTarget;
	DrawCommandBuffer* drawComBuffer;
}; 


