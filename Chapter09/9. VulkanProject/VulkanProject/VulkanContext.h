#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW\glfw3.h>

#include <vulkan\vulkan.h>

#include "AppValidationLayersAndExtensions.h"
#include "VulkanInstance.h"
#include "Device.h"


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


private:
	//surface
	VkSurfaceKHR surface;

	// My Classes
	AppValidationLayersAndExtensions *valLayersAndExt;
	VulkanInstance* vInstance;
	Device* device;


}; 


