#pragma once

#include "BaseFactory.h"
#include "../SingletonBase.h"

struct TextureType 
{
	~TextureType();

	unsigned int textureId;
	int width;
	int height;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};

class TextureFactory : public SingletonBase<TextureFactory>, public BaseFactory<TextureType>
{
public:
	virtual void DestroyFactory();

private:
	virtual TextureType* InsertNewType(const char* key);

	TextureType* LoadTexture(const char* textureDir);
	void GenerateMesh(TextureType* type);
};