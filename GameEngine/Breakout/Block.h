#pragma once
#include "../LObject.h"

class Block : public LObject
{
public:
	Block(Shader* shaderPtr, const std::string& tag, GLFWwindow* window = nullptr, const Transform& initialTransform = Transform()) :
		LObject(shaderPtr, tag, window, initialTransform)
	{}
};