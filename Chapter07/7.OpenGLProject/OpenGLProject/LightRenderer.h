#pragma once


#include <GL/glew.h>

#include "Dependencies/glm/glm/glm.hpp"
#include "Dependencies/glm/glm/gtc/type_ptr.hpp"

#include "Mesh.h"
#include "ShaderLoader.h";
#include "Camera.h"

class LightRenderer
{

public:
	LightRenderer(MeshType modelType, Camera* camera);
	~LightRenderer();

	void draw();

	void setPosition(glm::vec3 _position);
	void setColor(glm::vec3 _color);
	void setProgram(GLuint program);

	glm::vec3 getPosition();
	glm::vec3 getColor();


private:

	Camera* camera;

	std::vector<Vertex>vertices;
	std::vector<GLuint>indices;

	GLuint vbo, ebo, vao, program;
	
	glm::vec3 position, color;

};

