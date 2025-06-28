#include "CTextureRenderer.h"
#include "glad/glad.h"
#include "STBImage/stb_image.h"
#include <iostream>
#include "LObject.h"
#include "Flyweight/TextureFactory.h"

CTextureRenderer::CTextureRenderer(const char* textureFileDir)
{
	//Get texture data
	LoadTexture(textureFileDir, &_textureWidth, &_textureHeight);
	//GenerateMesh(&_textureWidth, &_textureHeight);
}

CTextureRenderer::~CTextureRenderer()
{
	/*
	//Delete object´s texture before the object gets destroyed
	glDeleteTextures(1, &_textureId);
	//Delete Vertex Array and Buffers
	glDeleteBuffers(1, &_VBO);
	glDeleteBuffers(1, &_EBO);
	glDeleteVertexArrays(1, &_VAO);
	*/
}

void CTextureRenderer::Update(float deltaTime)
{
	if (_texture == nullptr) return;

	//Enable blend and object´s texture
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture->textureId);

	//Draw mesh
	glBindVertexArray(_texture->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void CTextureRenderer::LoadTexture(const char* texDir, int* width, int* height)
{
	if (TextureFactory* factory = TextureFactory::GetInstance())
	{
		_texture = factory->GetType(texDir);
	}
	/*
	//Generate Texture
	glGenTextures(1, &_textureId);

	unsigned int format = 0;
	int nrComponents = 0;
	//Load image
	unsigned char* textureData = stbi_load(texDir, width, height, &nrComponents, 0);
	if (textureData)
	{
		//Check it´s format
		if (nrComponents == 1)
		{
			format = GL_RED;
		}
		if (nrComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}

		//If data is valid, then bind texture and set its data
		glBindTexture(GL_TEXTURE_2D, _textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, *width, *height, 0, format, GL_UNSIGNED_BYTE, textureData);
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
	stbi_image_free(textureData);*/
}

void CTextureRenderer::GenerateMesh(const int* width, const int* height)
{
	if (_texture == nullptr) return;

	//Generate vertex positions (pixel size) according to the texture
	/*float vertexData[16]
	{
		-(float)*width / 2.0f, (float)*height / 2.0f, 0.0f, 1.0f,
		(float)*width / 2.0f, (float)*height / 2.0f, 1.0f, 1.0f,
		(float)*width / 2.0f, -(float)*height / 2.0f, 1.0f, 0.0f,
		-(float)*width / 2.0f, -(float)*height / 2.0f, 0.0f, 0.0f
	};*/

	float vertexData[16]
	{
		-(float)_texture->width / 2.0f, (float)_texture->height / 2.0f, 0.0f, 1.0f,
		(float)_texture->width / 2.0f, (float)_texture->height / 2.0f, 1.0f, 1.0f,
		(float)_texture->width / 2.0f, -(float)_texture->height / 2.0f, 1.0f, 0.0f,
		-(float)_texture->width / 2.0f, -(float)_texture->height / 2.0f, 0.0f, 0.0f
	};

	//Generate indices to create EBO later
	unsigned int vertices[6]
	{
		0, 1, 2,
		3, 0, 2
	};

	//Set data for VAO, VBO and EBO
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
	glEnableVertexAttribArray(1);

	//Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}