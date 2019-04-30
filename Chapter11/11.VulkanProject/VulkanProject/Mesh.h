#include <vulkan\vulkan.h>
#include <vector>
#include <array>

#define GLM_FORCE_RADIAN
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

enum MeshType {

	kTriangle = 0,
	kQuad = 1,
	kCube = 2,
	kSphere = 3

};


struct UniformBufferObject {

	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;

};

struct Vertex {

	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoords;

	//-- Describes the rate at which rate to load data from memory
	//-- Specifies Number of bytes 
	//-- Whether to move to next data entry after each vertex or instance
	static VkVertexInputBindingDescription getBindingDescription() {

		VkVertexInputBindingDescription bindingDescription = {};

		bindingDescription.binding = 0; // binding location index
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}


	//-- How to handle vertex input

	static std::array<VkVertexInputAttributeDescription, 4> getAttributeDescriptions() {

		std::array<VkVertexInputAttributeDescription, 4> attributeDescriptions = {};

		attributeDescriptions[0].binding = 0; // from which binding the vertex comes --> it is 0 above
		attributeDescriptions[0].location = 0; // location layout in the vertex shader --> has 32 bit float components 
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT; // 
		attributeDescriptions[0].offset = offsetof(Vertex, pos); // number of bytes since the start of the per vertex data

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, normal);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, color);

		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[3].offset = offsetof(Vertex, texCoords);

		return attributeDescriptions;
	}
};

//const std::vector<Vertex> vertices = {
//	{ { -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } },
//	{ { 0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f },{ 0.0f, 0.0f } },
//	{ { 0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f },{ 0.0f, 1.0f } },
//	{ { -0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f },{ 1.0f, 1.0f } }
//};
//
//const std::vector<uint16_t> indices = {
//	0, 1, 2, 2, 3, 0
//};


//const std::vector<Vertex> vertices = {
//	{ { 0.0f, -0.5f },{ 1.0f, 1.0f, 0.0f } },
//	{ { 0.5f, 0.5f },{ 0.0f, 1.0f, 1.0f } },
//	{ { -0.5f, 0.5f },{ 1.0f, 0.0f, 1.0f } }
//};
//
//const std::vector<uint16_t> indices = {
//	
//	0, 1, 2
//};

/*
const std::vector<Vertex> vertices = {
{ { -0.5f, -0.5f },{ 1.0f, 0.0f, 0.0f } },
{ { 0.5f, -0.5f },{ 0.0f, 1.0f, 0.0f } },
{ { 0.5f, 0.5f },{ 0.0f, 0.0f, 1.0f } },
{ { -0.5f, 0.5f },{ 1.0f, 1.0f, 1.0f } }
};
*/


class Mesh {

public:
	static void setTriData(std::vector<Vertex>& vertices, std::vector<uint32_t>&indices);
	static void setQuadData(std::vector<Vertex>& vertices, std::vector<uint32_t>&indices);
	static void setCubeData(std::vector<Vertex>& vertices, std::vector<uint32_t>&indices);
	static void setSphereData(std::vector<Vertex>& vertices, std::vector<uint32_t>&indices);

};