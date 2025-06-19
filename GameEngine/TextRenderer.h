#pragma once

#include "glm/glm/glm.hpp"
#include <map>
#include <vector>
#include <iostream>

class Shader;

struct Character
{
	unsigned int vao;
	unsigned int textureId;
	glm::vec2 bearing;
	glm::vec2 size;
	unsigned int advance;
};

typedef unsigned int Font;

class TextRenderer
{
public:
	TextRenderer(float pixelFontSize, Shader* shaderPtr) : _shaderPtr(shaderPtr)
	{}

	Font LoadFont(const char* fontFileDir, float pixelSize);
	void RenderText(Font font, const char* text, glm::vec2 position, float textScale = 1.0f, glm::vec3 textColor = glm::vec3(1.0f));

private:
	Shader* _shaderPtr = nullptr;
	std::vector < std::map<char, Character>> _fonts;
};