#pragma once

#include "../LObject.h"

class Paddle : public LObject
{
public:
	Paddle(Shader* shaderPtr, const std::string& tag, const Transform& initialTransform = Transform()) :
		LObject(shaderPtr, tag, initialTransform)
	{}

	virtual void Update(float deltaTime) override;
};

