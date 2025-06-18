#pragma once

#include "glm/glm/glm.hpp"
#include <map>

class Shader;

struct Character
{
	unsigned int vao;
	unsigned int textureId;
	glm::vec2 bearing;
	glm::vec2 size;
	unsigned int advance;
};

class TextRenderer
{
public:
	TextRenderer(float pixelFontSize, Shader* shaderPtr);

	void RenderText(const char* text, glm::vec2 position, float textScale = 1.0f, glm::vec3 textColor = glm::vec3(1.0f));

private:
	void LoadFont(float pixelSize);

	Shader* _shaderPtr = nullptr;

	float _characterSize = 0.0f;
	std::map<char, Character> _characters;
};