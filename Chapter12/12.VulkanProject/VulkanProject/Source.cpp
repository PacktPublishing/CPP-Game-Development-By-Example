#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>

#include "VulkanContext.h"

#include "Camera.h"
#include "ObjectRenderer.h"

int main() {

	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "HELLO VULKAN ", nullptr, nullptr);

	VulkanContext::getInstance()->initVulkan(window);

	Camera camera;
	camera.init(45.0f, 1280.0f, 720.0f, 0.1f, 10000.0f);
	camera.setCameraPosition(glm::vec3(0.0f, 0.0f, 4.0f));

	ObjectRenderer tri;
	tri.createObjectRenderer(MeshType::kTriangle,
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(0.5f));

	ObjectRenderer quad;
	quad.createObjectRenderer(MeshType::kQuad,
		glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3(0.5f));

	ObjectRenderer cube;
	cube.createObjectRenderer(MeshType::kCube,
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(0.5f));

	ObjectRenderer sphere;
	sphere.createObjectRenderer(MeshType::kSphere,
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(0.5f));

	while (!glfwWindowShouldClose(window)) {

		VulkanContext::getInstance()->drawBegin();

		// draw command 
		tri.updateUniformBuffer(camera);
		quad.updateUniformBuffer(camera);
		cube.updateUniformBuffer(camera);
		sphere.updateUniformBuffer(camera);
		
		tri.draw();
		quad.draw();
		cube.draw();
		sphere.draw();

		VulkanContext::getInstance()->drawEnd();

		glfwPollEvents();
	}              

	tri.destroy();
	quad.destroy();
	cube.destroy();
	sphere.destroy();

	VulkanContext::getInstance()->cleanup();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}