#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include <string>

struct Shader
{
	GLuint ID;
	std::unordered_map<std::string, int> locations;
};