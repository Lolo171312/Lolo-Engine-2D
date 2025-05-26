#pragma once

#include "glm/glm/glm.hpp"

class LObject
{
public:
	LObject(const char* textureFileDir, const glm::vec3& initialLocation = glm::vec3(0.0f));
	~LObject();

	virtual void Update();

private:
	void LoadTexture(const char* texDir, int* width, int* height);
	void GenerateMesh(const int* width, const int* height);

	/*Transform Variables*/
	glm::vec3 _location;

	/*Texture Variables*/
	unsigned int _textureId;
	unsigned char* _textureData;

	/*Mesh Variables*/
	unsigned int _VAO;
};