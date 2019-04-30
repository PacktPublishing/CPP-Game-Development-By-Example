#include "Camera.h"

Camera::Camera(GLfloat FOV, GLfloat width, GLfloat height, GLfloat nearplane, GLfloat farPlane, glm::vec3 camPos){

	cameraPos = camPos;
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	viewMatrix = glm::lookAt(cameraPos, cameraFront, cameraUp);
	projectionMatrix = glm::perspective(FOV, width /height, nearplane, farPlane);
}


Camera::~Camera(){

}


glm::mat4 Camera::getViewMatrix() {

	return viewMatrix;
}
glm::mat4 Camera::getprojectionMatrix() {

	return projectionMatrix;
}

glm::vec3 Camera::getCameraPosition() {

	return cameraPos;
}


