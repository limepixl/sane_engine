#pragma once
#include <glad/glad.h>

struct Texture
{
	int width, height, channels;
	GLuint ID, index;

	static unsigned int GlobalTextureCount;
};

void BindTexture(Texture& texture);
void UnbindTexture();

void CleanTexture(Texture& t);