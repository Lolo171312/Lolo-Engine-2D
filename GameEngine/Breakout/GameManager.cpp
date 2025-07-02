#include "GameManager.h"
#include <cmath>
#include "Ball.h"
#include "Paddle.h"
#include "../TextRenderer.h"
#include "../ObjectsManager.h"

void GameManager::BeginPlay()
{
	if(_paddlePtr)
	{
		_paddleInitialPos = _paddlePtr->GetObjectLocation();
	}
	if(_ballPtr)
	{
		_ballInitialPos = _ballPtr->GetObjectLocation();
	}
}

void GameManager::UpdateManager(float deltaTime)
{
	//Render Health Text
	RenderText("Health:" + std::to_string(_health), glm::vec2(400.0f, 640.0f));
	//Render Points Text
	RenderText("Points:" + std::to_string(_points), glm::vec2(15.0f, 640.0f));
}

void GameManager::SetGameState(EGameStates newGameState)
{
	if(newGameState == LOSEHEALTH)
	{
		LoseHealth();
	}
	else if(newGameState == LOSEGAME)
	{
		RestartGame(false);
	}
	else if (newGameState == WINGAME)
	{
		RestartGame(true);
	}
}

void GameManager::LoseHealth()
{
	_health = glm::clamp(_health - 1, 0, 3); //Substract -1 health
	if (_health > 0) //Check if Health is greater than 0
	{
		if (_ballPtr && _paddlePtr)
		{
			//Set ball큦 position relative to paddle큦 position
			_ballPtr->SetObjectLocation(glm::vec2(_paddlePtr->GetObjectLocation().x, _paddlePtr->GetObjectLocation().y - 26.0f));
		}
	}
	else
	{
		SetGameState(LOSEGAME);
	}
}

void GameManager::RestartGame(bool wonGame)
{
	ObjectsManager::GetInstance()->EnableObjects(true);

	if(wonGame)
	{
		if (_ballPtr && _paddlePtr)
		{
			//Set ball큦 position relative to paddle큦 position
			_ballPtr->SetObjectLocation(glm::vec2(_paddlePtr->GetObjectLocation().x, _paddlePtr->GetObjectLocation().y - 26.0f));
		}

		SetPoints(100);
	}
	else
	{
		if (_paddlePtr)
		{
			_paddlePtr->SetObjectLocation(_paddleInitialPos);
		}
		if (_ballPtr)
		{
			_ballPtr->SetObjectLocation(_ballInitialPos);
		}

		_health = 3;
		_points = 0;
	}
}

void GameManager::DestroyManager()
{
	delete GameManager::GetInstance();
}
