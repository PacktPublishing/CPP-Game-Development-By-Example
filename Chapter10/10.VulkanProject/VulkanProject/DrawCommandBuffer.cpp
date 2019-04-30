#include "DrawCommandBuffer.h"

#include "VulkanContext.h"


DrawCommandBuffer::DrawCommandBuffer(){}

DrawCommandBuffer::~DrawCommandBuffer(){}

void DrawCommandBuffer::createCommandPoolAndBuffer(size_t imageCount){

	createCommandPool();
	allocateCommandBuffers(imageCount);
}

void DrawCommandBuffer::createCommandPool() {

	// Command pools can only submit command buffer of single type type of queue
	// Here we are setting the command pool to accept commands of type graphics

	QueueFamilyIndices qFamilyIndices = VulkanContext::getInstance()->getDevice()->getQueueFamiliesIndicesOfCurrentDevice();

	VkCommandPoolCreateInfo cpInfo = {};

	cpInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cpInfo.queueFamilyIndex = qFamilyIndices.graphicsFamily;
	cpInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

	if (vkCreateCommandPool(VulkanContext::getInstance()->getDevice()->logicalDevice, &cpInfo, nullptr, &commandPool) != VK_SUCCESS) {
		throw std::runtime_error(" failed to create command pool !!");
	}

}

// +++++++++++++++++++++++++++++++++++
// allocate command buffers from pool 
// +++++++++++++++++++++++++++++++++++

void DrawCommandBuffer::allocateCommandBuffers(size_t imageCount) {

	commandBuffers.resize(imageCount);

	VkCommandBufferAllocateInfo cbInfo = {};
	cbInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cbInfo.commandPool = commandPool;
	cbInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cbInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(VulkanContext::getInstance()->getDevice()->logicalDevice, &cbInfo, commandBuffers.data()) != VK_SUCCESS) {

		throw std::runtime_error(" failed to allocate command buffers !!");
	}

}

void DrawCommandBuffer::beginCommandBuffer(VkCommandBuffer commandBuffer){

	//printf(" record command buffer ! \n");

	VkCommandBufferBeginInfo cbBeginInfo = {};

	cbBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	// the command buffer can be resubmitted while it is already penfing for execution
	cbBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	cbBeginInfo.pInheritanceInfo = nullptr; // only relevant to seconday command buffers


	// ------- Begin command buffer recording

	if (vkBeginCommandBuffer(commandBuffer, &cbBeginInfo) != VK_SUCCESS) {

		throw std::runtime_error(" failed to begin command buffer !!");
	}

}

void DrawCommandBuffer::endCommandBuffer(VkCommandBuffer commandBuffer){

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {

		throw std::runtime_error(" failed to record command buffer");
	}

}

void DrawCommandBuffer::destroy(){

	vkDestroyCommandPool(VulkanContext::getInstance()->getDevice()->logicalDevice, commandPool, nullptr);
} // alloc command buffers function