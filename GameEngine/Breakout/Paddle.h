#pragma once

#include "../LObject.h"

class Paddle : public LObject
{
public:
	Paddle(Shader* shaderPtr, const std::string& tag, GLFWwindow* window = nullptr, const Transform& initialTransform = Transform());

	virtual void BeginPlay();
	virtual void Input(float deltaTime) override;

	virtual void OnEnable()
	{}
	virtual void OnDisable()
	{}

private:
	bool CheckWindowLimit(int pressingKey) const;

	float _speed = 380.0f;
	int _windowWidth = 0;
};

