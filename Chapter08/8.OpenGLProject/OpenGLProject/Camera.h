#pragma once


#include <GL/glew.h>

#include "Dependencies/glm/glm/glm.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"


class Camera
{
public:
	
	Camera(GLfloat FOV, GLfloat width, GLfloat height, GLfloat nearplane, GLfloat farPlane, glm::vec3 camPos);
	~Camera();

	glm::mat4 getViewMatrix();
	glm::mat4 getprojectionMatrix();
	glm::vec3 getCameraPosition();
	

private:


	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	glm::vec3 cameraPos;


};

