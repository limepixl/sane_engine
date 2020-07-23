#pragma once
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

struct Shader
{
	GLuint ID;
};

void SetFloat(float value, const char* location, Shader& shader);
void SetInt(int value, const char* location, Shader& shader);
void SetMat4(glm::mat4& value, const char* location, Shader& shader);