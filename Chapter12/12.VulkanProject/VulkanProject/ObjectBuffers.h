#pragma once

#include <vulkan\vulkan.h>
#include <vector>

#include "Mesh.h"

class ObjectBuffers
{
public:
	ObjectBuffers();
	~ObjectBuffers();

	std::vector<Vertex> vertices;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	std::vector<uint32_t> indices;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkBuffer uniformBuffers;
	VkDeviceMemory uniformBuffersMemory;

	void createVertexIndexUniformsBuffers(MeshType modelType);
	void destroy();

private:
	
	void createVertexBuffer();
	void createIndexBuffer();
	void createUniformBuffers();

};

