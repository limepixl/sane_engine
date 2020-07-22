#pragma once

struct Shader
{
	unsigned int ID;
};

Shader LoadFromFile(const char* vertexShaderPath, const char* fragmentShaderPath);