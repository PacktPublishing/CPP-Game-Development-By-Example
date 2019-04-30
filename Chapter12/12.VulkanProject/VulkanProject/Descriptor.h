#pragma once
#include <vulkan\vulkan.h>
#include <vector>

class Descriptor
{
public:
	Descriptor();
	~Descriptor();

	// all the descriptor bindings are combined into a single layout

	VkDescriptorSetLayout descriptorSetLayout; 
	VkDescriptorPool descriptorPool;
	VkDescriptorSet descriptorSet;

	void createDescriptorLayoutSetPoolAndAllocate(uint32_t _swapChainImageCount);
	void populateDescriptorSets(uint32_t _swapChainImageCount, VkBuffer uniformBuffers);

	void destroy();

private:

	void createDescriptorSetLayout();
	void createDescriptorPoolAndAllocateSets(uint32_t _swapChainImageCount);

};

