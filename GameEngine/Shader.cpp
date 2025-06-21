#include "Shader.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>

Shader::Shader(const char* vertexShader, const char* fragmentShader)
{
	char* vertexShaderCode = ReadShaderFile(vertexShader);
	if(vertexShaderCode == nullptr)
	{
		return;
	}

	char* fragmentShaderCode = ReadShaderFile(fragmentShader);
	if (fragmentShaderCode == nullptr)
	{
		return;
	}

	unsigned int vertexShaderId = CreateShader(vertexShaderCode, GL_VERTEX_SHADER);
	unsigned int fragmentShaderId = CreateShader(fragmentShaderCode, GL_FRAGMENT_SHADER);

	if (vertexShaderId == -1 || fragmentShaderId == -1) return;

	Id = glCreateProgram();
	glAttachShader(Id, vertexShaderId);
	glAttachShader(Id, fragmentShaderId);
	glLinkProgram(Id);

	int success;
	char infoLog[512];
	glGetProgramiv(Id, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(Id, 512, NULL, infoLog);
		std::cout << "ERROR: Shader Program Compilation error: " << infoLog << std::endl;
	}

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	free(vertexShaderCode);
	free(fragmentShaderCode);
}

Shader::~Shader()
{
	glDeleteProgram(Id);
}

void Shader::UseShader() const
{
	glUseProgram(Id);
}

unsigned int Shader::CreateShader(const char* shaderSourceCode, GLuint shaderType)
{
	unsigned int shaderId;
	shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &shaderSourceCode, NULL);
	glCompileShader(shaderId);

	int success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "ERROR:" << ((shaderType == GL_VERTEX_SHADER) ? "Vertex" : "Fragment") << " Shader Compilation error: " << infoLog << std::endl;
		return -1;
	}

	return shaderId;
}

char* Shader::ReadShaderFile(const char* shaderFileDir)
{
	FILE* shaderFile;
	fopen_s(&shaderFile, shaderFileDir, "rb");
	if (!shaderFile)
	{
		std::cout << "SHADER ERROR: Could not find shader" << std::endl;
		return nullptr;
	}

	fseek(shaderFile, 0, SEEK_END);
	long size = ftell(shaderFile);
	rewind(shaderFile);

	char* shaderBuffer = (char*)malloc(size + 1);
	if (!shaderBuffer)
	{
		std::cout << "SHADER ERROR: Could not create shader buffer" << std::endl;
		fclose(shaderFile);
		return nullptr;
	}

	fread(shaderBuffer, 1, size, shaderFile);
	shaderBuffer[size] = '\0';

	fclose(shaderFile);

	return shaderBuffer;
}

void Shader::SetFloatUniform(const char* uniformName, GLfloat newValue) const
{
	glUniform1f(glGetUniformLocation(Id, uniformName), newValue);
}

void Shader::SetIntUniform(const char* uniformName, int newValue) const
{
	glUniform1i(glGetUniformLocation(Id, uniformName), newValue);
}

void Shader::SetMatrix4Uniform(const char* uniformName, const GLfloat* newValue) const
{
	GLuint uniformId = glGetUniformLocation(Id, uniformName);
	glUniformMatrix4fv(uniformId, 1, GL_FALSE, newValue);
}

void Shader::SetVec3Uniform(const char* uniformName, const glm::vec3& newValue) const
{
	glUniform3fv(glGetUniformLocation(Id, uniformName), 1, &newValue[0]);
}