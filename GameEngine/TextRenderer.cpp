#include "TextRenderer.h"
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

TextRenderer::TextRenderer(float pixelFontSize, Shader* shaderPtr) : _shaderPtr(shaderPtr), _characterSize(pixelFontSize)
{
	if(_shaderPtr != nullptr)
	{
		_shaderPtr->UseShader();
	}

	LoadFont();

	float vertices[]
	{
		0.0f, 0.0f, 0.0f, 0.0f,
		pixelFontSize, 0.0f, 1.0f, 0.0f,
		pixelFontSize, pixelFontSize, 1.0f, 1.0f,
		0.0f, pixelFontSize, 0.0f, 1.0f
	};

	unsigned int indices[]
	{
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TextRenderer::RenderText(const char* text, glm::vec2 position)
{
	unsigned int index = 0;
	while (*(text + index) != '\0')
	{
		glm::mat4 translationMatrix = glm::mat4(1.0f);
		float xPos = 52.0f * index + position.x;
		translationMatrix = glm::translate(translationMatrix, glm::vec3(xPos, position.y, 0.0f));

		if (_shaderPtr != nullptr)
		{
			_shaderPtr->UseShader();
			_shaderPtr->SetMatrix4Uniform("model", glm::value_ptr(translationMatrix));
		}

		glActiveTexture(GL_TEXTURE0);

		Character characterData = _characters[*(text + index)];
		glBindTexture(GL_TEXTURE_2D, characterData.textureId);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		++index;
	}
}

void TextRenderer::LoadFont()
{
	FT_Library ft;
	if(FT_Init_FreeType(&ft))
	{
		std::cout << "Could not init FreeType Library" << std::endl;
		return;
	}

	FT_Face face;
	if(FT_New_Face(ft, "../Content/Fonts/basis33.ttf", 0, &face))
	{
		std::cout << "Could not load new font" << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	for (unsigned char c = 'a'; c <= 'z'; ++c)
	{
		if(FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "Could not load character" << std::endl;
			continue;
		}

		unsigned int characterTexture = 0;
		glGenTextures(1, &characterTexture);
		glBindTexture(GL_TEXTURE_2D, characterTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character newCharacter{
			characterTexture,
			glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			face->glyph->advance.x
		};

		_characters.insert(std::pair<char, Character>(c, newCharacter));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}