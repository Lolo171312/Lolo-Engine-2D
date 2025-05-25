#include "LObject.h"
#include "STBImage/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

LObject::LObject(const char* textureFileDir, const glm::vec3& initialLocation) : _location(initialLocation)
{
	//Generate Texture
	glGenTextures(1, &_textureId);

	//Get texture data
	int textureWidth, textureHeight;
	_textureData = stbi_load(textureFileDir, &textureWidth, &textureHeight, NULL, 4);
	if (_textureData) 
	{
		//If data is valid, then bind texture and set its data
		glBindTexture(GL_TEXTURE_2D, _textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, 0, textureWidth, textureHeight, 0, 0, GL_UNSIGNED_BYTE, _textureData);
		glGenerateMipmap(GL_TEXTURE_2D);

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

LObject::~LObject()
{
	//Delete object´s texture before the object gets destroyed
	glDeleteTextures(1, &_textureId);
}

void LObject::Update()
{
	//Draw mesh using the current texture and Shader
}