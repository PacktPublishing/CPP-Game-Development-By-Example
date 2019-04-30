#include <vector>

#include "Camera.h"
#include "LightRenderer.h"

#include <GL/glew.h>

#include "Dependencies/glm/glm/glm.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/glm/gtc/type_ptr.hpp"

#include <btBulletDynamicsCommon.h>

#include "LightRenderer.h"


#pragma once
	class MeshRenderer
	{
	public:
		MeshRenderer(MeshType modelType, std::string _name, Camera * _camera, btRigidBody* _rigidBody, LightRenderer* _light, float _specularStrength, float _ambientStrength);
		~MeshRenderer();
		
		 void draw();



		btRigidBody* rigidBody;

		void setPosition(glm::vec3 _position);
		void setScale(glm::vec3 _scale);
		void setProgram(GLuint _program);
		void setTexture(GLuint _textureID);

		std::string name = "";

	private:

		std::vector<Vertex>vertices;
		std::vector<GLuint>indices;
		glm::mat4 modelMatrix;


		Camera* camera;


		glm::vec3 position, scale;
		
		GLuint vao,	vbo, ebo, texture, program;

		LightRenderer* light;

		float ambientStrength, specularStrength;

	
	
	};


