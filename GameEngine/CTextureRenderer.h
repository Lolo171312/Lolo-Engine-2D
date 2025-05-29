#pragma once

#include "LComponent.h"
class CTextureRenderer : public LComponent
{
public:
	CTextureRenderer(const char* textureFileDir);
	~CTextureRenderer();
	virtual void Update(float deltaTime);

private:
	void LoadTexture(const char* texDir, int* width, int* height);
	void GenerateMesh(const int* width, const int* height);

	/*Texture Variables*/
	unsigned int _textureId;
	int _textureWidth;
	int _textureHeight;

	/*Mesh Variables*/
	unsigned int _VAO;
};

