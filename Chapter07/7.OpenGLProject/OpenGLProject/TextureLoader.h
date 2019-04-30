#pragma once
#include <string>
#include <GL/glew.h>

class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();

	GLuint getTextureID(std::string  texFileName);

};

