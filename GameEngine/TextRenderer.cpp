#include "TextRenderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

TextRenderer::~TextRenderer()
{
	delete _shaderPtr;
}

void TextRenderer::InitializeTextRenderer(const glm::vec2& windowSize)
{
	_shaderPtr = new Shader("Shaders/textShader.vs.txt", "Shaders/textShader.fs.txt");
	_shaderPtr->UseShader();
	glm::mat4 projection = glm::ortho(0.0f, windowSize.x, windowSize.y, 0.0f, 0.1f, 100.0f);
	_shaderPtr->SetMatrix4Uniform("projection", glm::value_ptr(projection));
	glUseProgram(0);

	_defaultFont = LoadFont("../Content/Fonts/basis33.ttf", 45.0f);
}

Font TextRenderer::LoadFontTR(const char* fontFileDir, float pixelSize)
{
	if (_shaderPtr != nullptr)
	{
		_shaderPtr->UseShader();
	}

	//Load FreeTrype Library
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		std::cout << "Could not init FreeType Library" << std::endl;
		return Font(-1, 0.0f);
	}

	//Load Font
	FT_Face face;
	if (FT_New_Face(ft, fontFileDir, 0, &face))
	{
		std::cout << "Could not load new font" << std::endl;
		return Font(-1, 0.0f);
	}

	//Determine current Font id
	unsigned int fontId = _fonts.size();

	//Set pixel size
	FT_Set_Pixel_Sizes(face, 0, pixelSize);

	std::map<char, Character> fontCharacters;
	//Iterate through every character and get their data
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 255; ++c)
	{
		//Load the current char
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "Could not load character" << std::endl;
			continue;
		}

		//Generate char texture
		unsigned int characterTexture = 0;
		glGenTextures(1, &characterTexture);
		glBindTexture(GL_TEXTURE_2D, characterTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Generate char mesh
		//Define vertices
		float vertices[]
		{
			0.0f, 0.0f, 0.0f, 0.0f,
			face->glyph->bitmap.width, 0.0f, 1.0f, 0.0f,
			face->glyph->bitmap.width, face->glyph->bitmap.rows, 1.0f, 1.0f,
			0.0f, face->glyph->bitmap.rows, 0.0f, 1.0f
		};
		//Define indices
		unsigned int indices[]
		{
			0, 1, 2,
			0, 2, 3
		};
		//Create mesh
		unsigned int VAO, VBO, EBO;
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

		//Create Character struct and fill it with the current char´s data
		Character newCharacter{
			VAO,
			characterTexture,
			glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			face->glyph->advance.x
		};

		//Insert the created struct in _character map asocciated with the current char
		fontCharacters.insert(std::pair<char, Character>(c, newCharacter));
	}

	_fonts.push_back(fontCharacters);

	//Every char data is loaded so we can unload FreeType´s Library and Font
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	return Font(_fonts.size() - 1, pixelSize);
}

void TextRenderer::RenderTextTR(Font font, const std::string& text, const glm::vec2& position, float textScale, const glm::vec3& textColor)
{
	if (_fonts.size() == 0)
	{
		font = _defaultFont;
	}
	if (font.id >= _fonts.size() || font.id < 0)
	{
		font = _defaultFont;
	}

	float x = position.x;
	float y = position.y;
	std::map<char, Character>& fontCharacters = _fonts[font.id];

	for (std::string::const_iterator c = text.begin(); c != text.end(); ++c)
	{
		if(*c == '\n')
		{
			x = position.x;
			y += font.pixelSize * textScale;
			continue;
		}

		Character characterData = fontCharacters[*(c)];

		float xPos = x + characterData.bearing.x * textScale;
		float yPos = y - characterData.bearing.y * textScale;
		glm::mat4 charModelMatrix = glm::mat4(1.0f);
		charModelMatrix = glm::translate(charModelMatrix, glm::vec3(xPos, yPos, 0.0f));
		charModelMatrix = glm::scale(charModelMatrix, glm::vec3(textScale, textScale, 1.0f));

		if (_shaderPtr != nullptr)
		{
			_shaderPtr->UseShader();
			_shaderPtr->SetMatrix4Uniform("model", glm::value_ptr(charModelMatrix));
			_shaderPtr->SetVec3Uniform("color", textColor);
		}

		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, characterData.textureId);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindVertexArray(characterData.vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		x += (characterData.advance >> 6) * textScale;
	}
}

#pragma region UserAccessFunctions
int InitTextRenderer(float windowWidth, float windowHeight)
{
	TextRenderer::Init();
	TextRenderer* instancePtr = TextRenderer::GetInstance();
	if (instancePtr != nullptr)
	{
		instancePtr->InitializeTextRenderer(glm::vec2(windowWidth, windowHeight));
		return 0;
	}

	return -1;
}

Font LoadFont(const char* fontFileDir, float pixelSize)
{
	TextRenderer* instancePtr = TextRenderer::GetInstance();
	if (instancePtr != nullptr)
	{
		return instancePtr->LoadFontTR(fontFileDir, pixelSize);
	}

	std::cout << "ERROR: TextRenderer was not initialized!" << std::endl;
	return Font(-1, 0.0f);
}

void RenderText(Font font, const std::string& text, const glm::vec2& position, float textScale, const glm::vec3& textColor)
{
	TextRenderer* instancePtr = TextRenderer::GetInstance();
	if (instancePtr != nullptr)
	{
		instancePtr->RenderTextTR(font, text, position, textScale, textColor);
		return;
	}

	std::cout << "ERROR: TextRenderer was not initialized!" << std::endl;
}

void RenderText(const std::string& text, const glm::vec2& position, float textScale, const glm::vec3& textColor)
{
	TextRenderer* instancePtr = TextRenderer::GetInstance();
	if (instancePtr != nullptr)
	{
		instancePtr->RenderTextTR(instancePtr->GetDefaultFont(), text, position, textScale, textColor);
		return;
	}

	std::cout << "ERROR: TextRenderer was not initialized!" << std::endl;
}
#pragma endregion UserAccessFunctions