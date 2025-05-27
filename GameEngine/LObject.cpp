#include "LObject.h"
#include "STBImage/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"

LObject::LObject(const char* textureFileDir, Shader* shaderPtr, const glm::vec2& initialLocation) : _location(initialLocation), _objectShader(shaderPtr)
{
	//Get texture data
	int textureWidth, textureHeight;
	LoadTexture(textureFileDir, &textureWidth, &textureHeight);
	GenerateMesh(&textureWidth, &textureHeight);
}

LObject::~LObject()
{
	//Delete object´s texture before the object gets destroyed
	glDeleteTextures(1, &_textureId);
}

void LObject::Update()
{
	//Enable shader if there is one
	if (_objectShader != nullptr)
	{
		_objectShader->UseShader();
		SetShaderModelMatrix();
	}

	//Enable blend and object´s texture
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	//Draw mesh
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void LObject::LoadTexture(const char* texDir, int* width, int* height)
{
	//Generate Texture
	glGenTextures(1, &_textureId);

	int nrComponents;
	_textureData = stbi_load(texDir, width, height, &nrComponents, 0);
	if (_textureData)
	{
		if (nrComponents == 1) 
		{
			_format = GL_RED;
		}
		if (nrComponents == 3) 
		{
			_format = GL_RGB;
		}
		else if (nrComponents == 4) 
		{
			_format = GL_RGBA;
		}

		//If data is valid, then bind texture and set its data
		glBindTexture(GL_TEXTURE_2D, _textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, _format, *width, *height, 0, _format, GL_UNSIGNED_BYTE, _textureData);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Unbind current texture
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else 
	{
		std::cout << "ERROR: No se pudo cargar la textura " << texDir << std::endl;
	}

	//Once the data has been set, free the loaded image
	stbi_image_free(_textureData);
}

void LObject::GenerateMesh(const int* width, const int* height)
{
	float vertexData[16]
	{
		0.0f, (float)*height, 0.0f, 1.0f,
		(float)*width, (float)*height, 1.0f, 1.0f,
		(float)*width, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
	};

	unsigned int vertices[6]
	{
		0, 1, 2,
		3, 0, 2
	};

	unsigned int VBO, EBO;
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*) (sizeof(float) * 2));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LObject::SetShaderModelMatrix()
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(_location, 0.0f));
	_objectShader->SetMatrix4Uniform("model", glm::value_ptr(modelMatrix));
}