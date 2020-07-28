#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>

struct Display
{
	char title[100];
	int width, height;
	GLFWwindow* window;
	float deltaTime, lastFrame, lastTime;
	float lastMouseX, lastMouseY;
	int numFrames;
};

Display CreateDisplay(int width, int height, const char* title);
void ProcessInput(Display& display, struct Camera& camera);
void DeltaTimeCalc(Display& display);