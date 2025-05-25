#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <cmath>
#include <iostream>

void Camera::Input(GLFWwindow* window, float deltaTime)
{
	CameraMovement(window, deltaTime);
	//FirstPersonMovement(window, deltaTime);
}

void Camera::MouseInput(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.9f) 
	{
		pitch = 89.9f;
	}
	if (pitch < -89.9f) 
	{
		pitch = -89.9f;
	}

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
	bodyFront = glm::vec3(cameraFront.x, 0.0f, cameraFront.z);
}

glm::mat4 Camera::GetViewMatrix()
{
	glm::mat4 viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::translate(viewMatrix, position);
	viewMatrix = glm::lookAt(position, position + cameraFront, cameraUp);
	//viewMatrix = MyLookAt(position, cameraUp, position + cameraFront);
	return viewMatrix;
}

void Camera::CameraMovement(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += cameraFront * speed * deltaTime;
		std::cout << "Position -> X: " << position.x << " | Y: " << position.y << " | Z: " << position.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= cameraFront * speed * deltaTime;
		std::cout << "Position -> X: " << position.x << " | Y: " << position.y << " | Z: " << position.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position -= glm::normalize(glm::cross(cameraUp, cameraFront)) * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position += glm::normalize(glm::cross(cameraUp, cameraFront)) * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		position += cameraUp * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		position -= cameraUp * speed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		fov += fovSpeed * deltaTime;
		if (fov > 90.0f)
		{
			fov = 90.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		fov -= fovSpeed * deltaTime;
		if (fov < 0.0f)
		{
			fov = 0.0f;
		}
	}
}

void Camera::FirstPersonMovement(GLFWwindow* window, float deltaTime)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += bodyFront * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= bodyFront * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed * deltaTime;
	}
}

glm::mat4 Camera::MyLookAt(const glm::vec3& position, const glm::vec3& upVector, const glm::vec3& target)
{
	glm::vec3 normalDirection = glm::normalize(position - target);
	glm::vec3 rightVector = glm::normalize(glm::cross(upVector, normalDirection));
	glm::vec3 newUpVector = glm::normalize(glm::cross(normalDirection, rightVector));
	
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	rotationMatrix[0][0] = rightVector.x;
	rotationMatrix[1][0] = rightVector.y;
	rotationMatrix[2][0] = rightVector.z;

	rotationMatrix[0][1] = newUpVector.x;
	rotationMatrix[1][1] = newUpVector.y;
	rotationMatrix[2][1] = newUpVector.z;

	rotationMatrix[0][2] = normalDirection.x;
	rotationMatrix[1][2] = normalDirection.y;
	rotationMatrix[2][2] = normalDirection.z;

	glm::mat4 translationMatrix = glm::mat4(1.0f);
	translationMatrix[3][0] = -position.x;
	translationMatrix[3][1] = -position.y;
	translationMatrix[3][2] = -position.z;

	return rotationMatrix * translationMatrix;
}
