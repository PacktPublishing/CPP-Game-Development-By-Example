#include "RenderTarget.h"

#include "VulkanContext.h"
#include "Tools.h"

RenderTarget::RenderTarget(){}

RenderTarget::~RenderTarget(){}

void RenderTarget::createViewsAndFramebuffer(std::vector<VkImage> swapChainImages, VkFormat swapChainImageFormat, VkExtent2D swapChainImageExtent, VkRenderPass renderPass){

	_swapChainImages =  swapChainImages;
	_swapChainImageExtent = swapChainImageExtent;

	createImageViews(swapChainImageFormat);
	createFrameBuffer(swapChainImageExtent, renderPass);
}

void RenderTarget::createImageViews(VkFormat swapChainImageFormat){

	swapChainImageViews.resize(_swapChainImages.size());

	for (size_t i = 0; i < _swapChainImages.size(); i++) {

		swapChainImageViews[i] = vkTools::createImageView(_swapChainImages[i],
			swapChainImageFormat, 
			VK_IMAGE_ASPECT_COLOR_BIT);
	}
}

void RenderTarget::createFrameBuffer(VkExtent2D swapChainImageExtent, VkRenderPass renderPass){

	swapChainFramebuffers.resize(swapChainImageViews.size());

	for (size_t i = 0; i < swapChainImageViews.size(); i++) {

		std::array<VkImageView, 1> attachments = {
			swapChainImageViews[i],
		};

		VkFramebufferCreateInfo fbInfo = {};
		fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		fbInfo.renderPass = renderPass; // needs the renderpass
		fbInfo.attachmentCount = static_cast<uint32_t>(attachments.size());;
		fbInfo.pAttachments = attachments.data();;
		fbInfo.width = swapChainImageExtent.width;
		fbInfo.height = swapChainImageExtent.height;
		fbInfo.layers = 1; // swapimages are single images so layers is equal to one

		if (vkCreateFramebuffer(VulkanContext::getInstance()->getDevice()->logicalDevice, &fbInfo, NULL, &swapChainFramebuffers[i]) != VK_SUCCESS) {

			throw std::runtime_error(" failed to create framebuffers !!!");
		}
	}
}


void RenderTarget::destroy(){

	// image views
	for (auto imageView : swapChainImageViews) {

		vkDestroyImageView(VulkanContext::getInstance()->getDevice()->logicalDevice, imageView, nullptr);
	}

	// Framebuffers
	for (auto framebuffer : swapChainFramebuffers) {
		vkDestroyFramebuffer(VulkanContext::getInstance()->getDevice()->logicalDevice, framebuffer, nullptr);
	}

}




