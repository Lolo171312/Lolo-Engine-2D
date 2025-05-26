#include "LObject.h"
#include "STBImage/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

LObject::LObject(const char* textureFileDir, const glm::vec3& initialLocation) : _location(initialLocation)
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(0);
	glBindTexture(0, _textureId);
	//Draw mesh using the current texture and Shader
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void LObject::LoadTexture(const char* texDir, int* width, int* height)
{
	//Generate Texture
	glGenTextures(1, &_textureId);

	_textureData = stbi_load(texDir, width, height, NULL, 4);
	if (_textureData)
	{
		//If data is valid, then bind texture and set its data
		glBindTexture(GL_TEXTURE_2D, _textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, *width, *height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _textureData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Unbind current texture
		glBindTexture(GL_TEXTURE_2D, 0);
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