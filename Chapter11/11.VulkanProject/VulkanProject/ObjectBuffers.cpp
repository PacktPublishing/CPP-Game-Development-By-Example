#include "ObjectBuffers.h"

#include "Tools.h"
#include "VulkanContext.h"


ObjectBuffers::ObjectBuffers(){}


ObjectBuffers::~ObjectBuffers(){}


void ObjectBuffers::createVertexIndexUniformsBuffers(MeshType modelType){


	switch (modelType) {

		case kTriangle: Mesh::setTriData(vertices, indices); break;
		case kQuad: Mesh::setQuadData(vertices, indices); break;
		case kCube: Mesh::setCubeData(vertices, indices); break;
		case kSphere: Mesh::setSphereData(vertices, indices); break;

	}

	 createVertexBuffer();
	 createIndexBuffer();
	 createUniformBuffers();

}

void ObjectBuffers::createVertexBuffer() {

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	//-- Staging buffer creation

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	vkTools::createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer,
		stagingBufferMemory);


	//-- Copying vertex data into the buffer
	//-- This is done with mapping the buffer memory

	void* data;

	//-- Allows us to access a region of the specified memory resource
	vkMapMemory(VulkanContext::getInstance()->getDevice()->logicalDevice,
		stagingBufferMemory,
		0, // offet
		bufferSize,// size
		0,// flag
		&data); // copy buffer memory to data 

	memcpy(data, vertices.data(), (size_t)bufferSize);

	// data may not be copied to the memory immediatly
	// Or writes to the buffer are not visible to mapped memory 
	// so we use VK_MEMORY_PROPERTY_HOST_COHERENT_BIT 
	// so that we can just call UnmapMemory

	vkUnmapMemory(VulkanContext::getInstance()->getDevice()->logicalDevice,
		stagingBufferMemory);

	// Create Vertex Buffer
	vkTools::createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, // not acceable my CPU and is optimal for the gpu to read from
		vertexBuffer,
		vertexBufferMemory);

	vkTools::copyBuffer(stagingBuffer,
		vertexBuffer,
		bufferSize);

	vkDestroyBuffer(VulkanContext::getInstance()->getDevice()->logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(VulkanContext::getInstance()->getDevice()->logicalDevice, stagingBufferMemory, nullptr);
}


// -- Create Index Buffer

void ObjectBuffers::createIndexBuffer() {

	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	vkTools::createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(VulkanContext::getInstance()->getDevice()->logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(VulkanContext::getInstance()->getDevice()->logicalDevice, stagingBufferMemory);

	vkTools::createBuffer(bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		indexBuffer, 
		indexBufferMemory);

	vkTools::copyBuffer(stagingBuffer,
		indexBuffer,
		bufferSize);

	vkDestroyBuffer(VulkanContext::getInstance()->getDevice()->logicalDevice, stagingBuffer, nullptr);
	vkFreeMemory(VulkanContext::getInstance()->getDevice()->logicalDevice, stagingBufferMemory, nullptr);

}

// -- Create Uniform buffer

void ObjectBuffers::createUniformBuffers() {

	VkDeviceSize bufferSize = sizeof(UniformBufferObject);

	vkTools::createBuffer(bufferSize,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			uniformBuffers,
			uniformBuffersMemory);

}

void ObjectBuffers::destroy(){


	vkDestroyBuffer(VulkanContext::getInstance()->getDevice()->logicalDevice, uniformBuffers, nullptr);
	vkFreeMemory(VulkanContext::getInstance()->getDevice()->logicalDevice, uniformBuffersMemory, nullptr);

	vkDestroyBuffer(VulkanContext::getInstance()->getDevice()->logicalDevice, indexBuffer, nullptr);
	vkFreeMemory(VulkanContext::getInstance()->getDevice()->logicalDevice, indexBufferMemory, nullptr);

	vkDestroyBuffer(VulkanContext::getInstance()->getDevice()->logicalDevice, vertexBuffer, nullptr);
	vkFreeMemory(VulkanContext::getInstance()->getDevice()->logicalDevice, vertexBufferMemory, nullptr);


}
