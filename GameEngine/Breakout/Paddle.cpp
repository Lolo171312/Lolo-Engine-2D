#include "Paddle.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Paddle::Paddle(Shader* shaderPtr, const std::string& tag, GLFWwindow* window, const Transform& initialTransform) : 
	LObject(shaderPtr, tag, window, initialTransform)
{
	if(window != nullptr)
	{
		glfwGetWindowSize(window, &_windowWidth, nullptr);
	}
}

void Paddle::Input(float deltaTime)
{
	LObject::Input(deltaTime);

	if(glfwGetKey(_window, GLFW_KEY_RIGHT)) //Right Arrow key Pressed
	{
		AddObjectLocation(glm::vec2(_speed * deltaTime, 0.0f)); //Right Movement
		//Check Right Margin
		if(CheckWindowLimit(GLFW_KEY_RIGHT))
		{
			/*
			* If the X pos of the object is greater than _windowWidth set its location to the largest right location
			* inside window space
			*/
			SetObjectLocation(glm::vec2(_windowWidth - 64, GetObjectLocation().y));
		}
	}

	if (glfwGetKey(_window, GLFW_KEY_LEFT)) //Left Arrow key Pressed
	{
		AddObjectLocation(glm::vec2(_speed * deltaTime * -1.0f, 0.0f)); //Left Movement
		//Check Right Margin
		if (CheckWindowLimit(GLFW_KEY_LEFT))
		{
			/*
			* If the X pos of the object is less than 0 set its location to the lowest left location
			* inside window space
			*/
			SetObjectLocation(glm::vec2(64.0f, GetObjectLocation().y));
		}
	}
}

bool Paddle::CheckWindowLimit(int pressingKey) const
{
	if(pressingKey == GLFW_KEY_RIGHT)
	{
		return GetObjectLocation().x + 64.0f > _windowWidth;
	}
	if(pressingKey == GLFW_KEY_LEFT)
	{
		return GetObjectLocation().x - 64.0f < 0.0f;
	}

	return false;
}
