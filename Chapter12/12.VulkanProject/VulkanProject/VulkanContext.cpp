#include "VulkanContext.h"


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

	// Create SwapChain
	swapChain = new SwapChain();
	swapChain->create(surface);

	renderPass = new Renderpass();
	renderPass->createRenderPass(swapChain->swapChainImageFormat);

	renderTarget = new RenderTarget();
	renderTarget->createViewsAndFramebuffer(swapChain->swapChainImages,
		swapChain->swapChainImageFormat,
		swapChain->swapChainImageExtent,
		renderPass->renderPass);

	drawComBuffer = new DrawCommandBuffer();
	drawComBuffer->createCommandPoolAndBuffer(swapChain->swapChainImages.size());


	// Synchronization

	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	// Fences are used to check draw command buffer completion
	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	// create it with the bit signaled so that at the drawframe
	// the bit will be signaled and be ready for rendering
	fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	vkCreateSemaphore(device->logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphore);
	vkCreateSemaphore(device->logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphore);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		if (vkCreateFence(device->logicalDevice, &fenceCreateInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

			throw std::runtime_error(" failed to create synchronization objects per frame !!");
		}
	}

}

void VulkanContext::drawBegin(){

	vkAcquireNextImageKHR(device->logicalDevice,
		swapChain->swapChain,
		std::numeric_limits<uint64_t>::max(),
		imageAvailableSemaphore, // is  signaled
		VK_NULL_HANDLE,
		&imageIndex);


	vkWaitForFences(device->logicalDevice, 1, &inFlightFences[imageIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());

	// Setting the fence back to unsignaled state
	vkResetFences(device->logicalDevice, 1, &inFlightFences[imageIndex]);


	currentCommandBuffer = drawComBuffer->commandBuffers[imageIndex];

	// Begin command buffer recording
	drawComBuffer->beginCommandBuffer(currentCommandBuffer);

	// Begin renderpass
	VkClearValue clearcolor = { 1.0f, 0.0f, 1.0f, 1.0f };

	std::array<VkClearValue, 1> clearValues = { clearcolor };

	renderPass->beginRenderPass(clearValues,
		currentCommandBuffer,
		renderTarget->swapChainFramebuffers[imageIndex],
		renderTarget->_swapChainImageExtent);
}

void VulkanContext::drawEnd(){

	// End render pass commands
	renderPass->endRenderPass(currentCommandBuffer);

	// End command buffer recording
	drawComBuffer->endCommandBuffer(currentCommandBuffer);



	// submit command buffer
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &currentCommandBuffer;

	// Wait for the stage that writes to color attachment
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	// Which stage of the pipeline to wait
	submitInfo.pWaitDstStageMask = waitStages;

	// Semaphore to wait on before submit command execution begins
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &imageAvailableSemaphore;			

	// Semaphore to be signaled when command buffers have completed
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &renderFinishedSemaphore;

	vkQueueSubmit(device->graphicsQueue, 1, &submitInfo, inFlightFences[imageIndex]);


	// Present frame
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &renderFinishedSemaphore; // set to wait

	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &swapChain->swapChain;
	presentInfo.pImageIndices = &imageIndex;

	vkQueuePresentKHR(device->presentQueue, &presentInfo);
	vkQueueWaitIdle(device->presentQueue);

}


void VulkanContext::cleanup() {


	vkDeviceWaitIdle(device->logicalDevice);

	vkDestroySemaphore(device->logicalDevice, renderFinishedSemaphore, nullptr);
	vkDestroySemaphore(device->logicalDevice, imageAvailableSemaphore, nullptr);

	// Fences and Semaphores
	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {

		vkDestroyFence(device->logicalDevice, inFlightFences[i], nullptr);
	}

	drawComBuffer->destroy();
	renderTarget->destroy();
	renderPass->destroy();
	swapChain->destroy();

	device->destroy();


	valLayersAndExt->destroy(vInstance->vkInstance, isValidationLayersEnabled);

	vkDestroySurfaceKHR(vInstance->vkInstance, surface, nullptr);	vkDestroyInstance(vInstance->vkInstance, nullptr);

}




// Getters and Setters


Device * VulkanContext::getDevice(){
	return device;
}

SwapChain * VulkanContext::getSwapChain() {

	return swapChain;
}

Renderpass * VulkanContext::getRenderpass() {

	return renderPass;
}

VkCommandBuffer VulkanContext::getCurrentCommandBuffer() {

	return currentCommandBuffer;
}
