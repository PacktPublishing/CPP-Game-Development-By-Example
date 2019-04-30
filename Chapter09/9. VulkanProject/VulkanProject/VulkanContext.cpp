#include "VulkanContext.h"
#include "Device.h"

VulkanContext* VulkanContext::instance = NULL;


VulkanContext* VulkanContext::getInstance() {

	if (!instance) {
		instance = new VulkanContext();
	}
	return instance;
}

VulkanContext::~VulkanContext(){

	if (instance) {
		delete instance;
		instance = nullptr;
	}
}

void VulkanContext::initVulkan(GLFWwindow* window) {


	// Validation and Extension Layers
	valLayersAndExt = new AppValidationLayersAndExtensions();

	if (isValidationLayersEnabled && !valLayersAndExt->checkValidationLayerSupport()) {
		throw std::runtime_error("alidation Layers Not Available !");
	}

	// Create App And Vulkan Instance()
	vInstance = new VulkanInstance();
	vInstance->createAppAndVkInstance(isValidationLayersEnabled, valLayersAndExt);

	// Debug CallBack
	valLayersAndExt->setupDebugCallback(isValidationLayersEnabled, vInstance->vkInstance);

	// Create Surface
	if (glfwCreateWindowSurface(vInstance->vkInstance, window, nullptr, &surface) != VK_SUCCESS) {

		throw std::runtime_error(" failed to create window surface !");
	}

	// PickPhysicalDevice and CreateLogicalDevice();
	device = new Device();
	device->pickPhysicalDevice(vInstance, surface);
	device->createLogicalDevice(surface, isValidationLayersEnabled, valLayersAndExt);

}


