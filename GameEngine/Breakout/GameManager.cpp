#include "GameManager.h"
#include <cmath>
#include "Ball.h"
#include "Paddle.h"
#include "../TextRenderer.h"
#include "../ObjectsManager.h"
#include "../CAudioSource.h"
#include "../Sound.h"

void GameManager::SetBackgroundColor(float r, float g, float b)
{
	_backgroundColor.x = r;
	_backgroundColor.y = g;
	_backgroundColor.z = b;
}

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

	_audioSource = new CAudioSource();
	_winClip = LoadSoundClip("../Content/Sounds/Win.wav");
	_loseClip = LoadSoundClip("../Content/Sounds/Lose.wav");
	_loseHealthClip = LoadSoundClip("../Content/Sounds/LoseHealth.wav");
}

void GameManager::UpdateManager(float deltaTime)
{
	//Render Health Text
	RenderText("Health:" + std::to_string(_health), glm::vec2(400.0f, 640.0f));
	//Render Points Text
	RenderText("Points:" + std::to_string(_points), glm::vec2(15.0f, 640.0f));

	if(_backgroundAnimation)
	{
		_backgroundAnimTimer += deltaTime;
		if(_backgroundAnimTimer > 0.1f)
		{
			_backgroundAnimTimer = 0.0f;
			++_backgroundAnimIndex;
			if(_backgroundAnimIndex % 2 != 0)
			{
				SetBackgroundColor(_backgroundInitialColor.x, _backgroundInitialColor.y, _backgroundInitialColor.z);
				if(_backgroundAnimIndex > 16)
				{
					_backgroundAnimation = false;
				}
			}
			else
			{
				SetBackgroundColor(_backgroundEndColor.x, _backgroundEndColor.y, _backgroundEndColor.z);
			}
		}
	}
}

void GameManager::StartBackgroundColorAnim(float r, float g, float b)
{
	_backgroundInitialColor = _backgroundColor;
	_backgroundEndColor.x = r;
	_backgroundEndColor.y = g;
	_backgroundEndColor.z = b;

	_backgroundAnimTimer = 0.0f;
	_backgroundAnimIndex = 0;
	_backgroundAnimation = true;

	SetBackgroundColor(r, g, b);
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
		StartBackgroundColorAnim(0.0f, 1.0f, 0.0f);
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

		StartBackgroundColorAnim(1.0f, 0.0f, 0.0f);

		if (_audioSource)
		{
			_audioSource->SetAudioClip(_loseHealthClip);
			_audioSource->Play();
		}
	}
	else
	{
		SetGameState(LOSEGAME);
		StartBackgroundColorAnim(0.0f, 0.0f, 0.0f);
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

		if(_audioSource)
		{
			_audioSource->SetAudioClip(_winClip);
			_audioSource->Play();
		}
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
			_ballPtr->SetSpeed(0.0f);
		}

		_health = 3;
		_points = 0;

		if (_audioSource)
		{
			_audioSource->SetAudioClip(_loseClip);
			_audioSource->Play();
		}
	}
}

void GameManager::DestroyManager()
{
	delete GameManager::GetInstance();
}
