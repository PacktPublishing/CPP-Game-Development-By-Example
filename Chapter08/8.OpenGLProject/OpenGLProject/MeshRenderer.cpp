
#include "MeshRenderer.h"


MeshRenderer::MeshRenderer(MeshType modelType, std::string _name, Camera* _camera, btRigidBody* _rigidBody, LightRenderer* _light, float _specularStrength, float _ambientStrength) {
	
	name = _name;
	rigidBody = _rigidBody;
	camera = _camera;
	light = _light;
	ambientStrength = _ambientStrength;
	specularStrength = _specularStrength;

	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.0, 0.0, 0.0);


	switch (modelType){
		case kTriangle: Mesh::setTriData(vertices, indices); break;
		case kQuad: Mesh::setQuadData(vertices, indices); break;
		case kCube: Mesh::setCubeData(vertices, indices);break;
		case kSphere: Mesh::setSphereData(vertices, indices); break;
	}


	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(),	&indices[0], GL_STATIC_DRAW);

	//Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::texCoords)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Vertex::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}


void MeshRenderer::draw() {



	modelMatrix = glm::mat4(1.0f);

	//** Manual 
	
	//glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f), position);

	//glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

	//modelMatrix = TranslationMatrix *scaleMatrix;

	 //**  Physics

	btTransform t;

	rigidBody->getMotionState()->getWorldTransform(t);

	btQuaternion rotation = t.getRotation();
	btVector3 translate = t.getOrigin();


	glm::mat4 RotationMatrix = glm::rotate(glm::mat4(1.0f), rotation.getAngle(),
		glm::vec3(rotation.getAxis().getX(),rotation.getAxis().getY(), rotation.getAxis().getZ()));

	glm::mat4 TranslationMatrix = glm::translate(glm::mat4(1.0f),
		glm::vec3(translate.getX(), translate.getY(), translate.getZ()));

	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);

	modelMatrix = TranslationMatrix * RotationMatrix * scaleMatrix;



	glm::mat4 vp = camera->getprojectionMatrix() * camera->getViewMatrix();

	// Set vp and Model matrix

	glUseProgram(this->program);
	
	GLint vpLoc = glGetUniformLocation(program, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(vp));

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// Set Texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set Lighting

	GLuint cameraPosLoc = glGetUniformLocation(program, "cameraPos");
	glUniform3f(cameraPosLoc, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

	GLuint lightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3f(lightPosLoc, this->light->getPosition().x, this->light->getPosition().y, this->light->getPosition().z);

	GLuint lightColorLoc = glGetUniformLocation(program, "lightColor");
	glUniform3f(lightColorLoc, this->light->getColor().x, this->light->getColor().y, this->light->getColor().z);

	GLuint specularStrengthLoc = glGetUniformLocation(program, "specularStrength");
	glUniform1f(specularStrengthLoc, specularStrength);

	GLuint ambientStrengthLoc = glGetUniformLocation(program, "ambientStrength");
	glUniform1f(ambientStrengthLoc, ambientStrength);


	glBindVertexArray(vao);		
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	
}


MeshRenderer::~MeshRenderer() {

}


// setters 

void MeshRenderer::setTexture(GLuint _textureID) {

	texture = _textureID;

}

void MeshRenderer::setScale(glm::vec3 _scale) {

	this->scale = _scale;
}


void MeshRenderer::setPosition(glm::vec3 _position) {

	this->position = _position;
}


void MeshRenderer::setProgram(GLuint _program) {

	this->program = _program;
}

