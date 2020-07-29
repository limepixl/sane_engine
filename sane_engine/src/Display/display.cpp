#include "display.h"
#include <Camera/camera.h>
#include <cstring>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Display CreateDisplay(int width, int height, const char* title)
{
	GLFWwindow* window;

	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	// Create GLFW window
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if(window == nullptr)
	{
		printf("Failed to create GLFW window!\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // TODO: Might not work on Linux

	// Initialize GLAD
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD!\n");
		glfwTerminate();
	}
	glViewport(0, 0, width, height);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	Display result = {"", width, height, window, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0};
	strcpy(result.title, title);
	return result;
}

// NOTE: Currently moves twice as fast when
// moving diagonally.
void ProcessInput(Display& display, Camera& camera)
{
	float cameraSpeed = 1.0f * display.deltaTime;
	if(glfwGetKey(display.window, GLFW_KEY_W) == GLFW_PRESS)
		camera.position += cameraSpeed * camera.forward;
	if(glfwGetKey(display.window, GLFW_KEY_S) == GLFW_PRESS)
		camera.position -= cameraSpeed * camera.forward;
	if(glfwGetKey(display.window, GLFW_KEY_A) == GLFW_PRESS)
		camera.position -= cameraSpeed * glm::normalize(glm::cross(camera.forward, camera.up));
	if(glfwGetKey(display.window, GLFW_KEY_D) == GLFW_PRESS)
		camera.position += cameraSpeed * glm::normalize(glm::cross(camera.forward, camera.up));
	if(glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.position.y += cameraSpeed;
	if(glfwGetKey(display.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.position.y -= cameraSpeed;

	double xpos, ypos;
	glfwGetCursorPos(display.window, &xpos, &ypos);

	float xoffset = (float)xpos - display.lastMouseX;
	float yoffset = display.lastMouseY - (float)ypos;
	display.lastMouseX = (float)xpos;
	display.lastMouseY = (float)ypos;

	float sens = 0.2f;
	xoffset *= sens;
	yoffset *= sens;

	camera.yaw += xoffset;
	camera.pitch += yoffset;
	UpdateVectors(camera);
}

void DeltaTimeCalc(Display& display)
{
	float currentFrame = (float)glfwGetTime();
	display.deltaTime = currentFrame - display.lastFrame;
	display.lastFrame = currentFrame;

	float delta = currentFrame - display.lastTime;
	display.numFrames++;
	if(delta >= 1.0f)
	{
		double frameTime = 1000.0 / (double)display.numFrames;
		double fps = (double)display.numFrames / delta;

		char buffer[100];
		sprintf(buffer, "%s | FPS: %.2lf | Frame Time: %.3lf", display.title, fps, frameTime);
		glfwSetWindowTitle(display.window, buffer);

		display.numFrames = 0;
		display.lastTime = currentFrame;
	}
}
