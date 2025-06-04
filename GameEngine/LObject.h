#pragma once

#include "glm/glm/glm.hpp"
#include <vector>

class Shader;
class LComponent;

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
	LObject(Shader* shaderPtr, const Transform& initialTransform = Transform());
	~LObject();

	void AttachComponent(LComponent* newComponent);
	void UpdateComponents(float deltaTime);

	virtual void Update(float deltaTime);

	/*
	* Transform Functions
	*/
	inline void SetObjectLocation(const glm::vec2& newLocation) { _transform._location = newLocation; };
	inline void SetObjectRotation(float newAngle) { _transform._angle = newAngle; };
	inline void SetObjectScale(const glm::vec2& newScale) { _transform._scale = newScale; };

	inline void AddObjectLocation(const glm::vec2& addLocation) { _transform._location += addLocation; }
	inline void AddObjectRotation(float addAngle) { _transform._angle += addAngle; }
	inline void AddObjectScale(const glm::vec2& addScale) { _transform._scale += addScale; }

	/*
	* Transform Getter Functions
	*/
	inline const glm::vec2& GetObjectLocation() const { return _transform._location; }
	inline float GetObjectAngle() const { return _transform._angle; }
	inline const glm::vec2& GetObjectScale() const { return _transform._scale; }
	inline const Transform& GetObjectTransform() const { return _transform; }

private:
	void LoadTexture(const char* texDir, int* width, int* height);
	void GenerateMesh(const int* width, const int* height);

	/*
	* Modifies Shader´s model matrix before the update according to the _transform values
	*/
	void SetShaderModelMatrix();

	/*Transform Variables*/
	Transform _transform;

	/*Texture Variables*/
	unsigned int _textureId;

	/*Mesh Variables*/
	unsigned int _VAO;

	/*Shader Variables*/
	Shader* _objectShader = nullptr;

	/*Vector of components owned by the object*/
	std::vector<LComponent*> _components;
};