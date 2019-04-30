#pragma once

#include <vulkan\vulkan.h>
#include <vector>

class DrawCommandBuffer
{
public:
	DrawCommandBuffer();
	~DrawCommandBuffer();

	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	void createCommandPoolAndBuffer(size_t imageCount);
	void beginCommandBuffer(VkCommandBuffer commandBuffer);
	void endCommandBuffer(VkCommandBuffer commandBuffer);

	void createCommandPool();
	void allocateCommandBuffers(size_t imageCount);

	void destroy();
};

