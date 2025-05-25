#pragma once

#include "glm/glm/glm.hpp"

class LObject
{
public:
	LObject(const char* textureFileDir, const glm::vec3& initialLocation = glm::vec3(0.0f));
	~LObject();

	virtual void Update();

private:
	/*Transform Variables*/
	glm::vec3 _location;

	/*Texture Variables*/
	unsigned int _textureId;
	unsigned char* _textureData;
};