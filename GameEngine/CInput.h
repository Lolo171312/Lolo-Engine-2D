#pragma once
#include "LComponent.h"

class GLFWwindow;

class CInput : public LComponent
{
public:
	CInput(GLFWwindow* window) : _window(window)
	{}

	virtual void Update(float deltaTime) = 0;

protected:
	GLFWwindow* _window;
};