#pragma once

#include "glm/glm/glm.hpp"
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include "SingletonBase.h"

class Shader;

struct Character
{
	unsigned int vao;
	unsigned int vbo;
	unsigned int ebo;
	unsigned int textureId;
	glm::vec2 bearing;
	glm::vec2 size;
	unsigned int advance;
};

//typedef unsigned int Font;
struct Font
{
	Font(unsigned int _id, float _size) : id(_id), pixelSize(_size)
	{}
	unsigned int id = 0;
	float pixelSize = 0.0f;
};

class TextRenderer : public SingletonBase<TextRenderer>
{
public:
	~TextRenderer();

	void InitializeTextRenderer(const glm::vec2& windowSize);
	Font LoadFontTR(const char* fontFileDir, float pixelSize);
	void RenderTextTR(Font font, const std::string& text, const glm::vec2& position, float textScale = 1.0f, const glm::vec3& textColor = glm::vec3(1.0f));

	inline Font GetDefaultFont() const { return _defaultFont; }

private:
	Shader* _shaderPtr = nullptr;
	std::vector < std::map<char, Character>> _fonts;

	Font _defaultFont = Font(-1, 0.0f);
};

/*
* User Access Functions
*/
int InitTextRenderer(float windowWidth, float windowHeight);
void DestroyTextRenderer();
Font LoadFont(const char* fontFileDir, float pixelSize);
void RenderText(Font font, const std::string& text, const glm::vec2& position, float textScale = 1.0f, const glm::vec3& textColor = glm::vec3(1.0f));
void RenderText(const std::string& text, const glm::vec2& position, float textScale = 1.0f, const glm::vec3& textColor = glm::vec3(1.0f));