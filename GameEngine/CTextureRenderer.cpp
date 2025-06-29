#include "CTextureRenderer.h"
#include "glad/glad.h"
#include "STBImage/stb_image.h"
#include <iostream>
#include "LObject.h"
#include "Flyweight/TextureFactory.h"
#include "Shader.h"

CTextureRenderer::CTextureRenderer(const char* textureFileDir)
{
	SetColor(glm::vec3(1.0f));
	//Get texture and mesh data
	LoadTextureAndMesh(textureFileDir);
}

void CTextureRenderer::Update(float deltaTime)
{
	if (_texture == nullptr) return;

	SetColorShader();

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

void CTextureRenderer::LoadTextureAndMesh(const char* texDir)
{
	if (TextureFactory* factory = TextureFactory::GetInstance())
	{
		_texture = factory->GetType(texDir);
	}
}

void CTextureRenderer::SetColorShader() const
{
	if (const LObject* owner = GetOwner()) 
	{
		if (const Shader* ownerShader = owner->GetShader()) 
		{
			ownerShader->SetVec3Uniform("color", _color);
		}
	}
}