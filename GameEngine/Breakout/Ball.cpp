#include "Ball.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include <cmath>
#include <iostream>
#include "../CCollider.h"
#include "GameManager.h"
#include "Paddle.h"
#include "../ObjectsManager.h"
#include "../Sound.h"
#include "../CAudioSource.h"

Ball::Ball(Shader* shaderPtr, const std::string& tag, GLFWwindow* window, const Transform& initialTransform) : 
	LObject(shaderPtr, tag, window, initialTransform)
{
	if (window != nullptr)
	{
		glfwGetWindowSize(window, &_windowWidth, &_windowHeight);
	}
}

void Ball::BeginPlay()
{
	_audioSourceCmp = GetComponent<CAudioSource>();
	_paddleBounceClip = LoadSoundClip("../Content/Sounds/PaddleImpact.wav");
	_launchBallClip = LoadSoundClip("../Content/Sounds/LaunchBall.wav");
	_bounceBorderClip = LoadSoundClip("../Content/Sounds/WindowBounce.wav");
}

void Ball::Update(float deltaTime)
{
	LObject::Update(deltaTime);

	if (!_isPlaying) return;

	AddObjectLocation(velocity * _speed * deltaTime);
	BallCollidesWithBorder();
}

void Ball::Input(float deltaTime)
{
	LObject::Input(deltaTime);

	if (_isPlaying) return;

	//Check input to Launch the ball
	if (glfwGetKey(_window, GLFW_KEY_SPACE) == GLFW_PRESS) //Space key Pressed
	{
		_isPlaying = true;
		velocity = glm::normalize(glm::vec2(0.0f, -1.0f));
		if(_audioSourceCmp)
		{
			_audioSourceCmp->SetAudioClip(_launchBallClip);
			_audioSourceCmp->Play();
		}
	} 

	//Move the ball alongside the Paddle
	if (glfwGetKey(_window, GLFW_KEY_RIGHT) == GLFW_PRESS) //Right Arrow key Pressed
	{
		AddObjectLocation(glm::vec2(_paddleSpeed * deltaTime, 0.0f)); //Right Movement
		//Check Right Margin
		if (CheckInitialMovementLimit(GLFW_KEY_RIGHT))
		{
			/*
			* If the X pos of the object is greater than _windowWidth set its location to the largest right location
			* inside window space
			*/
			SetObjectLocation(glm::vec2(_windowWidth - 64, GetObjectLocation().y));
		}
	}

	if (glfwGetKey(_window, GLFW_KEY_LEFT) == GLFW_PRESS) //Left Arrow key Pressed
	{
		AddObjectLocation(glm::vec2(_paddleSpeed * deltaTime * -1.0f, 0.0f)); //Left Movement
		//Check Right Margin
		if (CheckInitialMovementLimit(GLFW_KEY_LEFT))
		{
			/*
			* If the X pos of the object is less than 0 set its location to the lowest left location
			* inside window space
			*/
			SetObjectLocation(glm::vec2(64.0f, GetObjectLocation().y));
		}
	}
}

void Ball::OnCollisionEnter(CCollider* other)
{
	if (other == nullptr) return; //Check if other is nullptr

	if (LObject* otherObj = other->GetOwner()) //Check if other has Owner
	{
		if(otherObj->GetTag() == "Paddle") //Check if other is the Paddle
		{
			BallCollidesWithPaddle(otherObj);
			return;
		}

		BallCollidesWithBlock(otherObj);
	}
}

void Ball::SetSpeed(float newSpeed)
{
	_speed = glm::clamp(newSpeed, _minSpeed, _maxSpeed);
}

bool Ball::CheckInitialMovementLimit(int pressingKey) const
{
	if (pressingKey == GLFW_KEY_RIGHT)
	{
		return GetObjectLocation().x + 64.0f > _windowWidth;
	}
	if (pressingKey == GLFW_KEY_LEFT)
	{
		return GetObjectLocation().x - 64.0f < 0.0f;
	}

	return false;
}

void Ball::BallCollidesWithBorder()
{
	//Check if the ball is at the bottom of the screen
	//Lose Health condition
	if (GetObjectLocation().y - 16.0f > _windowHeight)
	{
		LoseHealth();
		return;
	}

	//Check if the ball is going further than any other margin
	//If that happens get the margin location
	float x, y;
	if (GetObjectLocation().x + 8.0f > _windowWidth)
	{
		x = GetObjectLocation().x + 8.0f;
	}
	else if (GetObjectLocation().x - 8.0f < 0.0f)
	{
		x = GetObjectLocation().x - 8.0f;
	}
	else
	{
		x = GetObjectLocation().x;
	}

	if (GetObjectLocation().y - 8.0f < 0.0f)
	{
		y = GetObjectLocation().y - 8.0f;
	}
	else
	{
		y = GetObjectLocation().y;
	}

	if (x != GetObjectLocation().x || y != GetObjectLocation().y)
	{
		//There is collision
		//Calculate the impact Normal in order to get the reflected vector
		glm::vec2 impactNormal = GetObjectLocation() - glm::vec2(x, y);
		impactNormal = glm::normalize(impactNormal);
		//Set ball´s velocity
		velocity = glm::normalize(glm::reflect(velocity, impactNormal));

		if(_audioSourceCmp)
		{
			_audioSourceCmp->SetAudioClip(_bounceBorderClip);
			_audioSourceCmp->Play();
		}
	}
}

void Ball::BallCollidesWithPaddle(LObject* paddle)
{
	//Set the velocity to the impact direction between the Ball and Paddle
	glm::vec2 impactDir = glm::normalize(GetObjectLocation() - paddle->GetObjectLocation());
	velocity = impactDir;

	if(_audioSourceCmp!=nullptr)
	{
		_audioSourceCmp->SetAudioClip(_paddleBounceClip);
		_audioSourceCmp->Play();
	}
}

void Ball::BallCollidesWithBlock(LObject* block)
{
	//Get the impact point between Ball and Block
	float width = 32.0f, height = 16.0f;
	glm::vec2 closestPoint;
	closestPoint.x = std::max(block->GetObjectLocation().x - width, std::min(GetObjectLocation().x, block->GetObjectLocation().x + width));
	closestPoint.y = std::max(block->GetObjectLocation().y - height, std::min(GetObjectLocation().y, block->GetObjectLocation().y + height));

	//Get the normal of the impact and calculate the reflect vector
	glm::vec2 impactNormal = glm::normalize(GetObjectLocation() - closestPoint);
	//Set the velocity to that reflected vector
	velocity = glm::normalize(glm::reflect(velocity, impactNormal));
	//Disable the object
	block->SetIsActive(false);
	GameManager::GetInstance()->SetPoints(50);
	SetSpeed(_speed + 10);

	const std::vector<LObject*>& worldObjs = ObjectsManager::GetInstance()->GetObjects();
	for (std::vector<LObject*>::const_iterator itr = worldObjs.begin(); itr != worldObjs.end(); ++itr)
	{
		if(*itr != nullptr && (*itr)->GetIsActive() && (*itr)->GetTag() == "Block")
		{
			return;
		}
	}

	_isPlaying = false;
	GameManager::GetInstance()->SetGameState(WINGAME);
}

void Ball::LoseHealth()
{
	_isPlaying = false;

	GameManager::GetInstance()->SetGameState(LOSEHEALTH);
}