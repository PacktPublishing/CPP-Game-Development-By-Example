#pragma once

#define GLM_FORCE_RADIAN
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Camera
{
public:
	
	void init(float FOV, float width, float height, float nearplane, float farPlane);
	void setCameraPosition(glm::vec3 position);
	glm::mat4 getViewMatrix();
	glm::mat4 getprojectionMatrix();

private:

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;
	glm::vec3 cameraPos;

};

