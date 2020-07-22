#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

struct Display
{
	int width, height;
	GLFWwindow* window;
};

Display CreateDisplay(int width, int height, const char* title);