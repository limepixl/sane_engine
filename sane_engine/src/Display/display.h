#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <cstdio>

struct Display
{
	char title[100];
	int width, height;
	GLFWwindow* window;

	// Used for delta time calc
	float deltaTime, lastFrame, lastTime;
	float lastMouseX, lastMouseY;
	int numFrames;
};

struct FBO_Data
{
	GLuint FBO;
	GLuint color;
	GLuint textureIndex;
	GLuint depth24stencil8;
};

Display CreateDisplay(int width, int height, const char* title);
void DeltaTimeCalc(Display& display);
void ProcessInput(Display& display, struct Camera& camera);
void CheckForResize(Display& display, FBO_Data& fbo, glm::mat4& projection);

FBO_Data CreateFBO(Display& display, struct Scene& scene);