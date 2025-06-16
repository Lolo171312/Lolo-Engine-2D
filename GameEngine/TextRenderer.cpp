#include "TextRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

TextRenderer::TextRenderer(float pixelFontSize, Shader* shaderPtr) : _shaderPtr(shaderPtr)
{
	if(_shaderPtr != nullptr)
	{
		_shaderPtr->UseShader();
	}

	float vertices[]
	{
		0.0f, 0.0f,
		pixelFontSize, 0.0f,
		pixelFontSize, pixelFontSize,
		0.0f, pixelFontSize
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2.0f, 0);

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

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		++index;
	}
}