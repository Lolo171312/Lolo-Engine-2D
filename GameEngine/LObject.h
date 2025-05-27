#pragma once

#include "glm/glm/glm.hpp"

class Shader;

class LObject
{
public:
	LObject(const char* textureFileDir, Shader* shaderPtr, const glm::vec2& initialLocation = glm::vec2(0.0f));
	~LObject();

	virtual void Update();

	inline void SetObjectWorldLocation(const glm::vec2& newLocation) { _location = newLocation; };

private:
	void LoadTexture(const char* texDir, int* width, int* height);
	void GenerateMesh(const int* width, const int* height);
	void SetShaderModelMatrix();

	/*Transform Variables*/
	glm::vec2 _location;

	/*Texture Variables*/
	unsigned int _textureId;
	unsigned char* _textureData;
	unsigned int _format;

	/*Mesh Variables*/
	unsigned int _VAO;

	/*Shader Variables*/
	Shader* _objectShader = nullptr;
};