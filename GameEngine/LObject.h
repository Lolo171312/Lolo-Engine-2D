#pragma once

#include "glm/glm/glm.hpp"
#include <vector>
#include <type_traits>
#include <string>

class Shader;
class LComponent;
class CCollider;
struct GLFWwindow;

struct Transform 
{
	Transform() : _location(glm::vec2(0.0f)), _angle(0.0f), _scale(glm::vec2(1.0f))
	{}


	//Transform(const glm::vec2& location, const glm::vec3& eulerAngles, const glm::vec2& scale) : _location(location), _eulerAngles(eulerAngles), _scale(scale)
	//{}
	//
	//Transform(float locationX = 0.0f, float locationY = 0.0f, float eulerX = 0.0f, float eulerY = 0.0f, float eulerZ = 0.0f, float scaleX = 1.0f, float scaleY = 1.0f) :
	//	_location(glm::vec2(locationX, locationY)),
	//	_eulerAngles(glm::vec3(eulerX, eulerY, eulerZ)),
	//	_scale(glm::vec2(scaleX, scaleY))
	//{}

	glm::vec2 _location;
	float _angle;
	glm::vec2 _scale;
};

class LObject
{
public:
	/*
	* Constructor and Destructor
	*/
	LObject(Shader* shaderPtr, const std::string& tag, GLFWwindow* window = nullptr, const Transform& initialTransform = Transform());
	~LObject();

	/*
	* Component Functions
	*/
	void AttachComponent(LComponent* newComponent);
	void UpdateComponents(float deltaTime);
	template<class T>
	T* GetComponent();

	virtual void Update(float deltaTime);
	virtual void Input(float deltaTime);

	virtual void OnCollisionEnter(CCollider* other)
	{}

	/*
	* Transform Functions
	*/
	inline void SetObjectLocation(const glm::vec2& newLocation) { _transform._location = newLocation; };
	inline void SetObjectAngle(float newAngle) { _transform._angle = newAngle; };
	inline void SetObjectScale(const glm::vec2& newScale) { _transform._scale = newScale; };

	inline void AddObjectLocation(const glm::vec2& addLocation) { _transform._location += addLocation; }
	inline void AddObjectAngle(float addAngle) { _transform._angle += addAngle; }
	inline void AddObjectScale(const glm::vec2& addScale) { _transform._scale += addScale; }

	/*
	* Transform Getter Functions
	*/
	inline const glm::vec2& GetObjectLocation() const { return _transform._location; }
	inline float GetObjectAngle() const { return _transform._angle; }
	inline const glm::vec2& GetObjectScale() const { return _transform._scale; }
	inline const Transform& GetObjectTransform() const { return _transform; }

	inline const Shader* GetShader() const { return _objectShader; }
	inline const std::string& GetTag() const { return _tag; }

	inline bool GetIsActive() const { return _isActive; }
	void SetIsActive(bool isActive);

protected:
	/*
	* Window variable
	* If we don´t want Input for this LObject this variable can remain as nullptr
	*/
	GLFWwindow* _window;

private:
	/*
	* Modifies Shader´s model matrix before the update according to the _transform values
	*/
	void SetShaderModelMatrix();

	/*Transform Variables*/
	Transform _transform;

	/*Shader Variables*/
	Shader* _objectShader = nullptr;

	/*Vector of components owned by the object*/
	std::vector<LComponent*> _components;

	/*Object tag*/
	std::string _tag = "None";

	/*
	* Is Object Active
	* If true -> Execute Update and Components Update
	* If false -> Early return in Update
	*/
	bool _isActive = true;
};

template<class T>
T* LObject::GetComponent()
{
	for (std::vector<LComponent*>::iterator itr = _components.begin(); itr != _components.end(); ++itr)
	{
		if(T* castedComponent = dynamic_cast<T*>(*itr))
		{
			return castedComponent;
		}
	}

	return nullptr;
}