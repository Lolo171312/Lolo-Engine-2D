#pragma once

#include "glm/glm/glm.hpp"

typedef unsigned int GLuint;
typedef float GLfloat;

class Shader
{
public:
	//Constructor and Destructor
	Shader(const char* vertexShader, const char* fragmentShader);
	~Shader();

	//Use this Shader Program
	void UseShader() const;

	//Set Program´s Variables 
	void SetFloatUniform(const char* uniformName, GLfloat newValue) const;
	void SetIntUniform(const char* uniformName, int newValue) const;
	void SetMatrix4Uniform(const char* uniformName, const GLfloat* newValue) const;
	void SetVec3Uniform(const char* uniformName, const glm::vec3& newValue) const;

private:
	unsigned int CreateShader(const char* shaderSourceCode, GLuint shaderType);
	char* ReadShaderFile(const char* shaderFileDir);

	unsigned int Id;
};

