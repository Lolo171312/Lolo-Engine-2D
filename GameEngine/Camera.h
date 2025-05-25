#pragma once

#include "glm/glm/glm.hpp"

struct GLFWwindow;

class Camera
{
public:
	void Input(GLFWwindow* window, float deltaTime);
	void MouseInput(GLFWwindow* window, double xpos, double ypos);

	glm::mat4 GetViewMatrix();

	inline float GetFov() const { return fov; }
	inline glm::vec3 GetPosition() const { return position; }

private:
	void CameraMovement(GLFWwindow* window, float deltaTime);
	void FirstPersonMovement(GLFWwindow* window, float deltaTime);

	glm::mat4 MyLookAt(const glm::vec3& position, const glm::vec3& upVector, const glm::vec3& target);

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	//First person Body Front Variable
	glm::vec3 bodyFront = glm::vec3(0.0f, 0.0f, -1.0f);

	float speed = 3.0f;
	float fovSpeed = 4.0f;
	float fov = 45.0f;

	float lastX = 400.0f;
	float lastY = 300.0f;

	float pitch = 0.0f;
	float yaw = 0.0f;
	bool firstMouse = true;
};

