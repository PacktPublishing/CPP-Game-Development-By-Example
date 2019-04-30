#version 450 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 vp;
uniform mat4 model;


void main(){

	gl_Position = vp * model *vec4(position, 1.0);

	TexCoord = texCoord;
}