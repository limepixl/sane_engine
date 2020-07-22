#pragma once
#include <glad/glad.h>

struct Shader
{
	GLuint ID;
};

Shader LoadShaderFromFile(const char* vertexShaderPath, const char* fragmentShaderPath);