#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 fragWorldPos;

uniform mat4 vp;
uniform mat4 model;


void main(){

	gl_Position = vp * model *vec4(position, 1.0);
	
	TexCoord = texCoord;
	Normal = mat3(transpose(inverse(model))) * normal;
	fragWorldPos = vec3(model * vec4(position, 1.0));
}