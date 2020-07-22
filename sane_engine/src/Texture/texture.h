#pragma once
#include <glad/glad.h>

struct Texture
{
	int width, height, channels;
	GLuint ID, index;
};

Texture LoadTextureFromFile(const char* path, unsigned int index);
void BindTexture(Texture* texture);