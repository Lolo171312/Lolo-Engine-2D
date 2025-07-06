#pragma once
#include "../LObject.h"

class CAudioSource;
struct AudioClipType;

class Ball : public LObject
{
public:
	Ball(Shader* shaderPtr, const std::string& tag, GLFWwindow* window = nullptr, const Transform& initialTransform = Transform());

	virtual void BeginPlay();
	virtual void Update(float deltaTime) override;
	virtual void Input(float deltaTime) override;

	virtual void OnCollisionEnter(CCollider* other) override;

	void SetSpeed(float newSpeed);

	virtual void OnEnable()
	{}
	virtual void OnDisable()
	{}

private:
	bool CheckInitialMovementLimit(int pressingKey) const;
	void BallCollidesWithBorder();
	void BallCollidesWithPaddle(LObject* paddle);
	void BallCollidesWithBlock(LObject* block);

	void LoseHealth();

	glm::vec2 velocity = glm::vec2(0.0f);

	float _paddleSpeed = 380.0f;
	float _speed = 350.0f;
	float _minSpeed = 350.0f;
	float _maxSpeed = 420.0f;
	int _windowWidth = 0;
	int _windowHeight = 0;
	bool _isPlaying = false;

	CAudioSource* _audioSourceCmp = nullptr;
	AudioClipType* _paddleBounceClip = nullptr;
	AudioClipType* _launchBallClip = nullptr;
	AudioClipType* _bounceBorderClip = nullptr;
};

