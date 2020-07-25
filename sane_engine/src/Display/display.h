#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

struct Display
{
	int width, height;
	GLFWwindow* window;
	float deltaTime, lastFrame;
	float lastMouseX, lastMouseY;
};

Display CreateDisplay(int width, int height, const char* title);
void ProcessInput(Display& display, struct Camera& camera);