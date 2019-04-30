#include "GraphicsPipeline.h"
#include "VulkanContext.h"

//#include <stdlib.h>

#include "Mesh.h"

GraphicsPipeline::GraphicsPipeline(){}

GraphicsPipeline::~GraphicsPipeline(){}


void GraphicsPipeline::createGraphicsPipelineLayoutAndPipeline(VkExtent2D swapChainImageExtent, VkDescriptorSetLayout descriptorSetLayout, VkRenderPass renderPass){

	createGraphicsPipelineLayout(descriptorSetLayout);
	createGraphicsPipeline(swapChainImageExtent, renderPass);

}

void GraphicsPipeline::createGraphicsPipelineLayout(VkDescriptorSetLayout descriptorSetLayout){

	// pipeline layout

   //used for sending uniform data to the shader
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

	// used for passing uniform objects and images to the shader
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout;

	if (vkCreatePipelineLayout(VulkanContext::getInstance()->getDevice()->logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {

		throw std::runtime_error(" failed to create pieline layout !");
	}

}


std::vector<char> GraphicsPipeline::readfile(const std::string& filename) {

	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open()) {
		throw std::runtime_error(" failed to open shader file");
	}
	size_t filesize = (size_t)file.tellg();
	std::vector<char> buffer(filesize);

	file.seekg(0);
	file.read(buffer.data(), filesize);

	file.close();

	return buffer;

}

VkShaderModule GraphicsPipeline::createShaderModule(const std::vector<char> & code) {

	VkShaderModuleCreateInfo cInfo = {};

	cInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	cInfo.codeSize = code.size();
	cInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(VulkanContext::getInstance()->getDevice()->logicalDevice, &cInfo, nullptr, &shaderModule) != VK_SUCCESS) {
		throw std::runtime_error(" failed to create shader module !");
	}

	return shaderModule;
}

void GraphicsPipeline::destroy(){

	vkDestroyPipeline(VulkanContext::getInstance()->getDevice()->logicalDevice, graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(VulkanContext::getInstance()->getDevice()->logicalDevice, pipelineLayout, nullptr);

}

void GraphicsPipeline::createGraphicsPipeline(VkExtent2D swapChainImageExtent,  VkRenderPass renderPass) {


	// vertex and fragment shader stage
	
	// vertex

	auto vertexShaderCode = readfile("shaders/SPIRV/basic.vert.spv");

	VkShaderModule vertexShadeModule = createShaderModule(vertexShaderCode);
	
	VkPipelineShaderStageCreateInfo vertShaderStageCreateInfo = {};
	vertShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageCreateInfo.module = vertexShadeModule;
	vertShaderStageCreateInfo.pName = "main";
	
	// fragment 

	auto fragmentShaderCode = readfile("shaders/SPIRV/basic.frag.spv");
	VkShaderModule fragShaderModule = createShaderModule(fragmentShaderCode);

	VkPipelineShaderStageCreateInfo fragShaderStageCreateInfo = {};

	fragShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageCreateInfo.module = fragShaderModule;
	fragShaderStageCreateInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageCreateInfo, fragShaderStageCreateInfo };

	// Vertex input State

	auto bindingDescription = Vertex::getBindingDescription();
	auto attribiteDescriptions = Vertex::getAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

	vertexInputInfo.vertexBindingDescriptionCount = 1; // intially was 0 as vertex data was hardcoded in the shader
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;

	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t> (attribiteDescriptions.size());
	vertexInputInfo.pVertexAttributeDescriptions = attribiteDescriptions.data();

	// Vertex Input assembly State
	
	VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
	inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyInfo.topology =  VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

	// Rasterization State
	
	VkPipelineRasterizationStateCreateInfo rastStateCreateInfo = {};
	rastStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rastStateCreateInfo.depthClampEnable = VK_FALSE;
	rastStateCreateInfo.rasterizerDiscardEnable = VK_FALSE; // if true geometry never passes through the rast stage and is never rendered
	rastStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rastStateCreateInfo.lineWidth = 1.0f;
	rastStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rastStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;// previously was clockwise
	rastStateCreateInfo.depthBiasEnable = VK_FALSE;
	rastStateCreateInfo.depthBiasConstantFactor = 0.0f;
	rastStateCreateInfo.depthBiasClamp = 0.0f;
	rastStateCreateInfo.depthBiasSlopeFactor = 0.0f;

	// Multisampling State
	VkPipelineMultisampleStateCreateInfo msStateInfo = {};
	msStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	msStateInfo.sampleShadingEnable = VK_FALSE;
	msStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	msStateInfo.minSampleShading = 1.0f;
	msStateInfo.pSampleMask = nullptr;
	msStateInfo.alphaToCoverageEnable = VK_FALSE;
	msStateInfo.alphaToOneEnable = VK_FALSE;

	// Depth and stencil test not required as no depth or stencil buffer !!!

	// Color Blend State
	VkPipelineColorBlendAttachmentState  cbAttach = {};
	cbAttach.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	cbAttach.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo cbCreateInfo = {};
	cbCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	cbCreateInfo.attachmentCount = 1;
	cbCreateInfo.pAttachments = &cbAttach;


	// Viewport State Create Info
	
	VkViewport viewport = {};
	viewport.x = 0;
	viewport.y = 0;
	viewport.width = (float)swapChainImageExtent.width;
	viewport.height = (float)swapChainImageExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = { 0,0 };
	scissor.extent = swapChainImageExtent;

	VkPipelineViewportStateCreateInfo vpStateInfo = {};
	vpStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	vpStateInfo.viewportCount = 1;
	vpStateInfo.pViewports = &viewport;
	vpStateInfo.scissorCount = 1;
	vpStateInfo.pScissors = &scissor;


	// Create Graphics Pipeline

	VkGraphicsPipelineCreateInfo gpInfo = {};
	gpInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	
	gpInfo.stageCount = 2;
	gpInfo.pStages = shaderStages;
	
	gpInfo.pVertexInputState = &vertexInputInfo;
	
	gpInfo.pInputAssemblyState = &inputAssemblyInfo;
	gpInfo.pViewportState = &vpStateInfo;
	gpInfo.pRasterizationState = &rastStateCreateInfo;
	gpInfo.pMultisampleState = &msStateInfo;
	gpInfo.pDepthStencilState = nullptr;
	gpInfo.pColorBlendState = &cbCreateInfo;
	gpInfo.pDynamicState = nullptr;

	gpInfo.layout = pipelineLayout;
	gpInfo.renderPass = renderPass;
	gpInfo.subpass = 0;

	gpInfo.basePipelineHandle = VK_NULL_HANDLE; // handle of existing pipeline to create a new one from it.
	gpInfo.basePipelineIndex = -1; // or reference another pipeline to be created.

	if (vkCreateGraphicsPipelines(VulkanContext::getInstance()->getDevice()->logicalDevice, VK_NULL_HANDLE, 1, &gpInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline !!");
	}

	vkDestroyShaderModule(VulkanContext::getInstance()->getDevice()->logicalDevice, vertexShadeModule, nullptr);
	vkDestroyShaderModule(VulkanContext::getInstance()->getDevice()->logicalDevice, fragShaderModule, nullptr);
}



