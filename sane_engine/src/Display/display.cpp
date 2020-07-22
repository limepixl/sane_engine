#include "display.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Display CreateDisplay(unsigned width, unsigned height, const char* title)
{
	Display tmp;

	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

	// Create GLFW window
	tmp.window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if(tmp.window == nullptr)
	{
		printf("Failed to create GLFW window!\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(tmp.window);
	glfwSetFramebufferSizeCallback(tmp.window, framebuffer_size_callback);

	// Initialize GLAD
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD!\n");
		glfwTerminate();
	}
	glViewport(0, 0, width, height);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

	return tmp;
}