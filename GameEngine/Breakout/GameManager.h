#pragma once

#include "../SingletonBase.h"
#include "glm/glm/glm.hpp"

class Paddle;
class Ball;

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

	void BeginPlay();
	void UpdateManager(float deltaTime);

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
};

