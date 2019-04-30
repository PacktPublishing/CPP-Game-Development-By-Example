#pragma once

#include "GraphicsPipeline.h"
#include "ObjectBuffers.h"
#include "Descriptor.h"

#include "Camera.h"

class ObjectRenderer
{
public:

	void createObjectRenderer(MeshType modelType, glm::vec3 _position, glm::vec3 _scale);
	void draw();
	void updateUniformBuffer(Camera camera);
	void destroy();

private:


	GraphicsPipeline gPipeline;
	ObjectBuffers objBuffers;
	Descriptor descriptor;

	glm::vec3 position;
	glm::vec3 scale;

};

