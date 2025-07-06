#pragma once

#include "../SingletonBase.h"
#include "glm/glm/glm.hpp"

class Paddle;
class Ball;
class CAudioSource;
struct AudioClipType;

enum EGameStates
{
	PLAYING, LOSEHEALTH, LOSEGAME, WINGAME
};

class GameManager : public SingletonBase<GameManager>
{
public:
	inline void SetPaddlePtr(Paddle* paddlePtr) { _paddlePtr = paddlePtr; }
	inline Paddle* GetPaddlePtr() const { return _paddlePtr; }
	inline void SetBallPtr(Ball* ballPtr) { _ballPtr = ballPtr; }
	inline Ball* GetBallPtr() const { return _ballPtr; }
	inline void SetPoints(int points) { _points += points; }
	inline const glm::vec3& GetBackgroundColor() { return _backgroundColor; }
	void SetBackgroundColor(float r, float g, float b);

	void BeginPlay();
	void UpdateManager(float deltaTime);

	void StartBackgroundColorAnim(float r, float g, float b);

	/*
	* GameState
	*/
	void SetGameState(EGameStates newGameState);
	void LoseHealth();
	void RestartGame(bool wonGame);

	void DestroyManager();

private:
	Paddle* _paddlePtr = nullptr;
	glm::vec2 _paddleInitialPos;
	Ball* _ballPtr = nullptr;
	glm::vec2 _ballInitialPos;
	int _health = 3;
	int _points = 0;
	glm::vec3 _backgroundColor;

	glm::vec3 _backgroundInitialColor;
	glm::vec3 _backgroundEndColor;
	bool _backgroundAnimation = false;
	float _backgroundAnimTimer = 0.0f;
	int _backgroundAnimIndex = 0;

	CAudioSource* _audioSource = nullptr;
	AudioClipType* _winClip = nullptr;
	AudioClipType* _loseClip = nullptr;
	AudioClipType* _loseHealthClip = nullptr;
};

