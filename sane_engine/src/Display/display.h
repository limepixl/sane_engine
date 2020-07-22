#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

struct Display
{
	unsigned width, height;
	GLFWwindow* window;
};

Display CreateDisplay(unsigned width, unsigned height, const char* title);