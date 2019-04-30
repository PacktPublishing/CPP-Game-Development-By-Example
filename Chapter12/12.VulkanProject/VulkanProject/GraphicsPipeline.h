#pragma once

#include <vulkan\vulkan.h>
#include <vector>

#include <fstream>

class GraphicsPipeline
{
public:
	GraphicsPipeline();
	~GraphicsPipeline();

	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	void createGraphicsPipelineLayoutAndPipeline(VkExtent2D swapChainImageExtent, VkDescriptorSetLayout descriptorSetLayout, VkRenderPass renderPass);

	void destroy();

private:

	std::vector<char> readfile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char> & code);

	void createGraphicsPipelineLayout(VkDescriptorSetLayout descriptorSetLayout);
	void createGraphicsPipeline(VkExtent2D swapChainImageExtent, VkRenderPass renderPass);

};

