#pragma once

#include "glm/glm/glm.hpp"
#include <map>

class Shader;

struct Character
{
	unsigned int textureId;
	glm::vec2 bearing;
	glm::vec2 size;
	unsigned int advance;
};

class TextRenderer
{
public:
	TextRenderer(float pixelFontSize, Shader* shaderPtr);

	void RenderText(const char* text, glm::vec2 position);

private:
	void LoadFont();

	Shader* _shaderPtr = nullptr;

	unsigned int _textureId = 0;
	unsigned int VBO = 0, VAO = 0, EBO = 0;

	float _characterSize = 0.0f;
	std::map<char, Character> _characters;
};