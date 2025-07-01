#pragma once
#include "../LObject.h"

class Ball : public LObject
{
public:
	Ball(Shader* shaderPtr, const std::string& tag, GLFWwindow* window = nullptr, const Transform& initialTransform = Transform());

	virtual void Update(float deltaTime) override;
	virtual void Input(float deltaTime) override;

	virtual void OnCollisionEnter(CCollider* other) override;

private:
	bool CheckInitialMovementLimit(int pressingKey) const;
	void BallCollidesWithBorder();

	void LoseHealth();

	glm::vec2 velocity = glm::vec2(0.0f);

	float _paddleSpeed = 320.0f;
	float _speed = 350.0f;
	int _windowWidth = 0;
	int _windowHeight = 0;
	bool _isPlaying = false;

	int _health = 3;
};

