#version 450 core

in vec2 TexCoord;

out vec4 color;

// texture
uniform sampler2D Texture;


void main(){
		
		color = texture(Texture, TexCoord);
		
}