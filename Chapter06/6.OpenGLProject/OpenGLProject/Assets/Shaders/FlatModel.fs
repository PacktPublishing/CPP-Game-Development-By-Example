#version 450 core

in vec3 outColor;

out vec4 color;

void main(){
		
	color = vec4(outColor, 1.0f);

}