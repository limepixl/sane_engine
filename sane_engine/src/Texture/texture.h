#pragma once
#include <glad/glad.h>

struct Texture
{
	int width, height, channels;
	GLuint ID, index;
};

void BindTexture(Texture& texture);
void UnbindTexture();

void Clean(Texture& t);