#include "LObject.h"
#include "STBImage/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "LComponent.h"

LObject::LObject(Shader* shaderPtr, const Transform& initialTransform) : _transform(initialTransform), _objectShader(shaderPtr)
{
	//Get texture data
	//int textureWidth, textureHeight;
	//LoadTexture(textureFileDir, &textureWidth, &textureHeight);
	//GenerateMesh(&textureWidth, &textureHeight);
}

LObject::~LObject()
{
	//Delete object´s texture before the object gets destroyed
	glDeleteTextures(1, &_textureId);
}

void LObject::AttachComponent(LComponent* newComponent)
{
	if (newComponent) 
	{
		newComponent->AttachComponent(this);
		_components.push_back(newComponent);
	}
}

void LObject::UpdateComponents(float deltaTime)
{
	if (_components.size() == 0) return;

	for (std::vector<LComponent*>::const_iterator itr = _components.begin(); itr != _components.end(); ++itr)
	{
		if ((*itr)->GetIsActive()) 
		{
			(*itr)->Update(deltaTime);
		}
	}
}

void LObject::Update(float deltaTime)
{
	//Enable shader if there is one
	if (_objectShader != nullptr)
	{
		_objectShader->UseShader();
		SetShaderModelMatrix();
	}

	UpdateComponents(deltaTime);

	////Enable blend and object´s texture
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, _textureId);
	//
	////Draw mesh
	//glBindVertexArray(_VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
}

void LObject::LoadTexture(const char* texDir, int* width, int* height)
{
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
	stbi_image_free(textureData);
}

void LObject::GenerateMesh(const int* width, const int* height)
{
	//Generate vertex positions (pixel size) according to the texture
	float vertexData[16]
	{
		-(float)*width/2.0f, (float)*height / 2.0f, 0.0f, 1.0f,
		(float)*width / 2.0f, (float)*height / 2.0f, 1.0f, 1.0f,
		(float)*width / 2.0f, -(float)*height / 2.0f, 1.0f, 0.0f,
		-(float)*width / 2.0f, -(float)*height / 2.0f, 0.0f, 0.0f
	};

	//Generate indices to create EBO later
	unsigned int vertices[6]
	{
		0, 1, 2,
		3, 0, 2
	};

	//Create and set data for VAO, VBO and EBO
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

	//Unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void LObject::SetShaderModelMatrix()
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(_transform._location, 0.0f)); //Translate
	modelMatrix = glm::rotate(modelMatrix, glm::radians(_transform._angle), glm::vec3(0.0f, 0.0f, 1.0f)); //Rotation (Only front axis)
	modelMatrix = glm::scale(modelMatrix, glm::vec3(_transform._scale, 1.0f)); //Scale

	//Set model matrix in the Shader
	_objectShader->SetMatrix4Uniform("model", glm::value_ptr(modelMatrix)); 
}