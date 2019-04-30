#include "TextRenderer.h"

// https://github.com/ubawurinna/freetype-windows-binaries

TextRenderer::TextRenderer(std::string text, std::string font, int size, glm::vec3 color, GLuint program){
	
	this->text = text;
	this->color = color;
	this->scale = 1.0;

	this->program = program;

	this->setPosition(position);

	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(800), 0.0f, static_cast<GLfloat>(600));
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	
	// FreeType
	FT_Library ft;

	// Initialise freetype
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
	
	// Load font
	FT_Face face;
	if (FT_New_Face(ft, font.c_str(), 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size of glyphs
	FT_Set_Pixel_Sizes(face, 0, size);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	// Load first 128 characters of ASCII set
	// From the loaded font

	for (GLubyte i = 0; i < 128; i++){

		// Load character glyph 
		if (FT_Load_Char(face, i, FT_LOAD_RENDER)){
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);

		// Set texture filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
	
		// Create a character
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		
		// Store character in characters map
		Characters.insert(std::pair<GLchar, Character>(i, character));
	}


	glBindTexture(GL_TEXTURE_2D, 0);

	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
		
}


TextRenderer::~TextRenderer(){
}

void TextRenderer::draw(){
	

	glm::vec2 textPos = this->position;

	//enable blending

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set program, text color and texture
	glUseProgram(program);
	glUniform3f(glGetUniformLocation(program, "textColor"), this->color.x, this->color.y, this->color.z);
	glActiveTexture(GL_TEXTURE0);
	
	glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator c;

	for (c = text.begin(); c != text.end(); c++){

		Character ch = Characters[*c];

		GLfloat xpos = textPos.x + ch.Bearing.x * this->scale;
		GLfloat ypos = textPos.y - (ch.Size.y - ch.Bearing.y) * this->scale;

		GLfloat w = ch.Size.x * this->scale;
		GLfloat h = ch.Size.y * this->scale;
		
		// Per Character Update VBO
		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },

			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		// Use glBufferSubData and not glBufferData
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		// Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		textPos.x += (ch.Advance >> 6) * this->scale; 
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);

}

void TextRenderer::setPosition(glm::vec2 _position){

	this->position = _position;
}


void TextRenderer::setText(std::string _text){

	this->text = _text;

}


