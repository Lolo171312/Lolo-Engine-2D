#pragma once

#include "LComponent.h"
#include "glm/glm/glm.hpp"

struct TextureType;

class CTextureRenderer : public LComponent
{
public:
	CTextureRenderer(const char* textureFileDir);
	virtual void Update(float deltaTime);

	inline void SetColor(const glm::vec3& newColor) { _color = newColor; }
	inline const glm::vec3& GetColor() const { return _color; }

private:
	void LoadTextureAndMesh(const char* texDir);

	void SetColorShader() const;

	TextureType* _texture;
	glm::vec3 _color;
};

