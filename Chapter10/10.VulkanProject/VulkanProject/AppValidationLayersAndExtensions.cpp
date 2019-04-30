#include "AppValidationLayersAndExtensions.h"


AppValidationLayersAndExtensions::AppValidationLayersAndExtensions(){}


AppValidationLayersAndExtensions::~AppValidationLayersAndExtensions(){}

// validation layers check if the correct parameters are passed to functions, etc
// check if all the layers are supported

//--checkValidationlayerSupport()

bool AppValidationLayersAndExtensions::checkValidationLayerSupport() {

	uint32_t layerCount;
	// Get count of validation layers available
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	// Get the available validation layers names 
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : requiredValidationLayers) { //layers we require

		// boolean to check if the layer was found
		bool layerFound = false;

		for (const auto& layerproperties : availableLayers) {

			// If layer is found set the layar found boolean to true
			if (strcmp(layerName, layerproperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}

		return true;

	}

}//check validation layer support


 // For sending the debug info to the program we need VK_debug_report extention
 // So check if the extension is supported

 // --getRequiredExtensions()

std::vector<const char*> AppValidationLayersAndExtensions::getRequiredExtensions(bool isValidationLayersEnabled) {

	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	// Get extensions
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	//debug report extention is added.

	if (isValidationLayersEnabled) {
		extensions.push_back("VK_EXT_debug_report");
	}

	return extensions;
}

//callback function to display the callback message

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
	VkDebugReportFlagsEXT flags,
	VkDebugReportObjectTypeEXT objExt,
	uint64_t obj,
	size_t location,
	int32_t code,
	const char* layerPrefix,
	const char* msg,
	void* userData) {

	std::cerr << "validation layer: " << msg << std::endl << std::endl;

	return false;

}


void AppValidationLayersAndExtensions::setupDebugCallback(bool isValidationLayersEnabled, VkInstance vkInstance) {

	if (!isValidationLayersEnabled) {
		return;
	}

	printf("setup call back \n");

	VkDebugReportCallbackCreateInfoEXT info = {};

	info.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	info.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	info.pfnCallback = debugCallback; // callback function

	if (createDebugReportCallbackEXT(vkInstance, &info, nullptr, &callback) != VK_SUCCESS) {

		throw std::runtime_error("failed to set debug callback!");
	}

}

void AppValidationLayersAndExtensions::destroy(VkInstance instance, bool isValidationLayersEnabled){

	if (isValidationLayersEnabled) {
		DestroyDebugReportCallbackEXT(instance, callback, nullptr);
	}

}
