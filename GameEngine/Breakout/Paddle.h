#pragma once

#include "../LObject.h"

class Paddle : public LObject
{
public:
	Paddle(Shader* shaderPtr, const std::string& tag, GLFWwindow* window = nullptr, const Transform& initialTransform = Transform());

	virtual void Input(float deltaTime) override;

private:
	bool CheckWindowLimit(int pressingKey) const;

	float _speed = 320.0f;
	int _windowWidth = 0;
};

