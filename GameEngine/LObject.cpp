#include "LObject.h"
#include "STBImage/stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "LComponent.h"
#include "CCollider.h"

LObject::~LObject()
{
	//Delete every created component before the objects gets destroyed
	for (std::vector<LComponent*>::iterator itr = _components.begin(); itr != _components.end(); ++itr) 
	{
		delete (*itr);
	}
}

void LObject::AttachComponent(LComponent* newComponent)
{
	if (newComponent) 
	{
		newComponent->AttachComponent(this);
		_components.push_back(newComponent);
	}
}

void LObject::UpdateComponents(float deltaTime)
{
	if (_components.size() == 0) return;

	for (std::vector<LComponent*>::const_iterator itr = _components.begin(); itr != _components.end(); ++itr)
	{
		if ((*itr)->GetIsActive()) 
		{
			(*itr)->Update(deltaTime);
		}
	}
}

void LObject::Update(float deltaTime)
{
	//Enable shader if there is one
	if (_objectShader != nullptr)
	{
		_objectShader->UseShader();
		SetShaderModelMatrix();
	}

	UpdateComponents(deltaTime);
}

void LObject::OnCollisionEnter(CCollider* other)
{

}

void LObject::SetShaderModelMatrix()
{
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(_transform._location, 0.0f)); //Translate
	modelMatrix = glm::rotate(modelMatrix, glm::radians(_transform._angle), glm::vec3(0.0f, 0.0f, 1.0f)); //Rotation (Only front axis)
	modelMatrix = glm::scale(modelMatrix, glm::vec3(_transform._scale, 1.0f)); //Scale

	//Set model matrix in the Shader
	_objectShader->SetMatrix4Uniform("model", glm::value_ptr(modelMatrix)); 
}