#include "Renderpass.h"
#include "VulkanContext.h"
#include "Tools.h"

Renderpass::Renderpass(){}


Renderpass::~Renderpass(){}


void Renderpass::createRenderPass(VkFormat swapChainImageFormat) {

	// Tell vulkan the swapchain framebuffer attachments we will be using
	// How many color buffers and depth buffers and 
	// How many samples to use for each of them;

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; //clear the values to a constant at the start
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //rendered contents will be stored in memory and can be read later.

	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;

	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // images are sent to the swapchain

	// Subpasses - used for post processing
	// previous attachment is sent as reference to subpass to be worked on

	VkAttachmentReference colorAttachRef = {};
	colorAttachRef.attachment = 0; // the first attachment at 0th index
	colorAttachRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // images used as color attachment

	// Subpass
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;//this is a graphics subpass could be compute as well
	subpass.colorAttachmentCount = 1; // this references the - layout(location = 0) out vec4 outColor - of the fragment shader
	subpass.pColorAttachments = &colorAttachRef;


	// Create render pass

	std::array<VkAttachmentDescription, 1> attachments = { colorAttachment };

	VkRenderPassCreateInfo rpCreateInfo = {};
	rpCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rpCreateInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
	rpCreateInfo.pAttachments = attachments.data();
	rpCreateInfo.subpassCount = 1;
	rpCreateInfo.pSubpasses = &subpass;


	if (vkCreateRenderPass(VulkanContext::getInstance()->getDevice()->logicalDevice, &rpCreateInfo, nullptr, &renderPass) != VK_SUCCESS) {
		throw std::runtime_error(" failed to create renderpass !!");
	}
}

void Renderpass::beginRenderPass(std::array<VkClearValue, 1> clearValues,
	VkCommandBuffer commandBuffer,
	VkFramebuffer swapChainFrameBuffer,
	VkExtent2D swapChainImageExtent) {

	VkRenderPassBeginInfo rpBeginInfo = {};
	rpBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	rpBeginInfo.renderPass = renderPass;
	rpBeginInfo.framebuffer = swapChainFrameBuffer;//sChain->swapChainFramebuffers[i];
	rpBeginInfo.renderArea.offset = { 0,0 };
	rpBeginInfo.renderArea.extent = swapChainImageExtent;//sChain->swapChainImageExtent;


	rpBeginInfo.pClearValues = clearValues.data();
	rpBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());

	// ------ Begin the render pass
	vkCmdBeginRenderPass(commandBuffer,// commandBuffers[i]
		&rpBeginInfo,
		VK_SUBPASS_CONTENTS_INLINE);
}

void Renderpass::endRenderPass(VkCommandBuffer commandBuffer) {

	vkCmdEndRenderPass(commandBuffer);
}

void Renderpass::destroy(){
	
	vkDestroyRenderPass(VulkanContext::getInstance()->getDevice()->logicalDevice, renderPass, nullptr);

}
