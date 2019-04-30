#include "ObjectRenderer.h"

#include "VulkanContext.h"



void ObjectRenderer::createObjectRenderer(MeshType modelType, glm::vec3 _position, glm::vec3 _scale){
	

	uint32_t swapChainImageCount = VulkanContext::getInstance()->getSwapChain()->swapChainImages.size();
	VkExtent2D swapChainImageExtent = VulkanContext::getInstance()->getSwapChain()->swapChainImageExtent;



	// Create Vertex, Index and Uniforms Buffer;
	objBuffers.createVertexIndexUniformsBuffers(modelType);

	// CreateDescriptorSetLayout
	descriptor.createDescriptorLayoutSetPoolAndAllocate(swapChainImageCount);
	descriptor.populateDescriptorSets(swapChainImageCount,
												  objBuffers.uniformBuffers);

	// CreateGraphicsPipeline
	gPipeline.createGraphicsPipelineLayoutAndPipeline(
		swapChainImageExtent,
		descriptor.descriptorSetLayout,
		VulkanContext::getInstance()->getRenderpass()->renderPass);

	
	position = _position;
	scale = _scale;
}

void ObjectRenderer::draw(){

	VkCommandBuffer cBuffer = VulkanContext::getInstance()->getCurrentCommandBuffer();

	// Bind the pipeline
	vkCmdBindPipeline(cBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, gPipeline.graphicsPipeline);

	// Bind vertex buffer to command buffer
	VkBuffer vertexBuffers[] = { objBuffers.vertexBuffer };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(cBuffer,
		0, // first binding
		1, // binding count
		vertexBuffers,
		offsets);

	// Bind index buffer to the command buffer
	vkCmdBindIndexBuffer(cBuffer,
		objBuffers.indexBuffer,
		0,
		VK_INDEX_TYPE_UINT32);

	//	Bind uniform buffer using descriptorSets
	vkCmdBindDescriptorSets(cBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		gPipeline.pipelineLayout,
		0,
		1,
		&descriptor.descriptorSet, 0, nullptr);

	vkCmdDrawIndexed(cBuffer,
		static_cast<uint32_t>(objBuffers.indices.size()), // no of indices
		1, // instance count -- just the 1
		0, // first index -- start at 0th index
		0, // vertex offet -- any offsets to add
		0);// first instance -- since no instancing, is set to 0 

}

void ObjectRenderer::updateUniformBuffer(Camera camera){

	UniformBufferObject ubo = {};

	glm::mat4 scaleMatrix = glm::mat4(1.0f);
	glm::mat4 rotMatrix = glm::mat4(1.0f);
	glm::mat4 transMatrix = glm::mat4(1.0f);

	scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	transMatrix = glm::translate(glm::mat4(1.0f), position);

	ubo.model = transMatrix * rotMatrix * scaleMatrix;

	ubo.view = camera.getViewMatrix();
	
	ubo.proj = camera.getprojectionMatrix();

	ubo.proj[1][1] *= -1; // invert Y as in Opengl it is inverted to begin with

	void* data;
	vkMapMemory(VulkanContext::getInstance()->getDevice()->logicalDevice, objBuffers.uniformBuffersMemory, 0, sizeof(ubo), 0, &data);

	memcpy(data, &ubo, sizeof(ubo));

	vkUnmapMemory(VulkanContext::getInstance()->getDevice()->logicalDevice, objBuffers.uniformBuffersMemory);
	
}

void ObjectRenderer::destroy() {

	gPipeline.destroy();
	descriptor.destroy();
	objBuffers.destroy();
}

