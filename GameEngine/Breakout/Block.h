#pragma once
#include "../LObject.h"

class CAudioSource;
struct AudioClipType;

class Block : public LObject
{
public:
	Block(Shader* shaderPtr, const std::string& tag, GLFWwindow* window = nullptr, const Transform& initialTransform = Transform()) :
		LObject(shaderPtr, tag, window, initialTransform)
	{}

	virtual void BeginPlay();

	virtual void OnEnable()
	{}
	virtual void OnDisable();

private:
	CAudioSource* _audioSourceCmp = nullptr;
	AudioClipType* _breakClip = nullptr;
};