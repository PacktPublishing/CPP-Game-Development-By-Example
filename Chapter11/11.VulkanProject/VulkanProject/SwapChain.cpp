#include "SwapChain.h"
#include "VulkanContext.h"

SwapChain::SwapChain(){}

SwapChain::~SwapChain(){}

void SwapChain::create(VkSurfaceKHR surface) {

	SwapChainSupportDetails swapChainSupportDetails = VulkanContext::getInstance()->getDevice()->swapchainSupport;

	VkSurfaceFormatKHR surfaceFormat = chooseSwapChainSurfaceFormat(swapChainSupportDetails.surfaceFormats);
	VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupportDetails.presentModes);
	VkExtent2D extent = chooseSwapExtent(swapChainSupportDetails.surfaceCapabilities);

	//set number of swap chain images
	uint32_t imageCount = swapChainSupportDetails.surfaceCapabilities.minImageCount;

	if (swapChainSupportDetails.surfaceCapabilities.maxImageCount > 0 && imageCount > swapChainSupportDetails.surfaceCapabilities.maxImageCount) {
		imageCount = swapChainSupportDetails.surfaceCapabilities.maxImageCount;
	}

	// creation of swapchain info struct
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1; // this is 1 unless you are making a stereoscopic 3D application
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = VulkanContext::getInstance()->getDevice()->getQueueFamiliesIndicesOfCurrentDevice();

	uint32_t queueFamilyIndices[] = { (uint32_t)indices.graphicsFamily, (uint32_t)indices.presentFamily };

	if (indices.graphicsFamily != indices.presentFamily) {

		// image can be used across multiple queue families without ownership transfer
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;

	}
	else {

		// image is owned by one family at a time and ownership has to be transferred
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0;
		createInfo.pQueueFamilyIndices = nullptr;
	}

	// transform can be applied to the images if required otherwise choose current transform
	createInfo.preTransform = swapChainSupportDetails.surfaceCapabilities.currentTransform;

	// ignore blending with other windows in the window system
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

	createInfo.presentMode = presentMode;

	// do you want pixels to be clipped if there is a window in front
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(VulkanContext::getInstance()->getDevice()->logicalDevice, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain !");
	}


	// Retrieve swapchain images
	// Color images are created automatically

	vkGetSwapchainImagesKHR(VulkanContext::getInstance()->getDevice()->logicalDevice, swapChain, &imageCount, nullptr);
	swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(VulkanContext::getInstance()->getDevice()->logicalDevice, swapChain, &imageCount, swapChainImages.data());

	swapChainImageFormat = surfaceFormat.format;
	swapChainImageExtent = extent;

}// createSwapChain


void SwapChain::destroy(){

	// Swapchain
	vkDestroySwapchainKHR(VulkanContext::getInstance()->getDevice()->logicalDevice, swapChain, nullptr);
	
}


// ++++++++++++++++++
// Swap chain helpers
// ++++++++++++++++++

//choosing the right setting for the surfaceformat, presentation mode and swap extent

VkSurfaceFormatKHR SwapChain::chooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {

	if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
		return{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	}

	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			return availableFormat;
		}
	}
	return availableFormats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes) {

	VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

	for (const auto & availablePresentMode : availablePresentModes) {

		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
		else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
			bestMode = availablePresentMode;
		}

		return bestMode;
	}
}


VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) {

	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	}
	else {

		VkExtent2D actualExtent = { 800, 600 };

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}
