#include "TextureFactory.h"
#include "glad/glad.h"
#include "STBImage/stb_image.h"
#include <iostream>

void TextureFactory::DestroyFactory()
{
	delete TextureFactory::GetInstance();
}

TextureType* TextureFactory::InsertNewType(const char* key)
{
	return LoadTexture(key);
}

TextureType* TextureFactory::LoadTexture(const char* textureDir)
{
	TextureType* newType = new TextureType();

	//Generate Texture
	glGenTextures(1, &newType->textureId);

	unsigned int format = 0;
	int nrComponents = 0;
	//Load image
	unsigned char* textureData = stbi_load(textureDir, &(newType->width), &(newType->height), &nrComponents, 0);
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
		glBindTexture(GL_TEXTURE_2D, newType->textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, newType->width, newType->height, 0, format, GL_UNSIGNED_BYTE, textureData);
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
		std::cout << "ERROR: No se pudo cargar la textura " << newType->textureId << std::endl;
	}

	//Once the data has been set, free the loaded image
	stbi_image_free(textureData);

	GenerateMesh(newType);

	return newType;
}

void TextureFactory::GenerateMesh(TextureType* type)
{
	if (type == nullptr) return;

	float vertexData[16]
	{
		-(float)type->width / 2.0f, (float)type->height / 2.0f, 0.0f, 1.0f,
		(float)type->width / 2.0f, (float)type->height / 2.0f, 1.0f, 1.0f,
		(float)type->width / 2.0f, -(float)type->height / 2.0f, 1.0f, 0.0f,
		-(float)type->width / 2.0f, -(float)type->height / 2.0f, 0.0f, 0.0f
	};

	//Generate indices to create EBO later
	unsigned int vertices[6]
	{
		0, 1, 2,
		3, 0, 2
	};

	//Set data for VAO, VBO and EBO
	glGenVertexArrays(1, &type->VAO);
	glBindVertexArray(type->VAO);

	glGenBuffers(1, &type->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, type->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glGenBuffers(1, &type->EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, type->EBO);
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

TextureType::~TextureType()
{
	//Delete object´s texture before the object gets destroyed
	glDeleteTextures(1, &textureId);
	//Delete Vertex Array and Buffers
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}