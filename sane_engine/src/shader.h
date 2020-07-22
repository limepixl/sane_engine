#pragma once
#include <glad/glad.h>

struct Shader
{
	GLuint ID;
};

Shader LoadFromFile(const char* vertexShaderPath, const char* fragmentShaderPath);