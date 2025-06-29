#pragma once

#include "../LObject.h"

class Paddle : public LObject
{
public:
	Paddle(Shader* shaderPtr, const std::string& tag, GLFWwindow* window = nullptr, const Transform& initialTransform = Transform()) :
		LObject(shaderPtr, tag, window, initialTransform)
	{}

	virtual void Input(float deltaTime) override;
};

