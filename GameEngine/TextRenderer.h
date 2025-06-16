#pragma once

#include "glm/glm/glm.hpp"

class Shader;

class TextRenderer
{
public:
	TextRenderer(float pixelFontSize, Shader* shaderPtr);

	void RenderText(const char* text, glm::vec2 position);

private:
	Shader* _shaderPtr = nullptr;

	unsigned int _textureId = 0;
	unsigned int VBO = 0, VAO = 0, EBO = 0;
};