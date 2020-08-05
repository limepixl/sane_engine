#include "display.h"
#include <Camera/camera.h>
#include <Scene/scene.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cstring>

Display CreateDisplay(int width, int height, const char* title)
{
	GLFWwindow* window;

	// Initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE); // TODO: Definitely doesn't work on Linux

	// Create GLFW window
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if(window == nullptr)
	{
		printf("Failed to create GLFW window!\n");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
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

	Display result = {"", width, height, window, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0 };
	strcpy(result.title, title);
	return result;
}

// NOTE: Currently moves twice as fast when
// moving diagonally.
void ProcessInput(Display& display, Camera& camera)
{
	// Keyboard movement
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

	// Mouse movement / looking around
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
	
	// Fixes able to look upside down
	if(camera.pitch > 89.0f)
		camera.pitch = 89.0f;
	if(camera.pitch < -89.0f)
		camera.pitch = -89.0f;

	camera.forward.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	camera.forward.y = sin(glm::radians(camera.pitch));
	camera.forward.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	camera.forward = glm::normalize(camera.forward);
}

void CheckForResize(Display& display, FBO_Data& fbo, glm::mat4& projection)
{
	int w, h;
	glfwGetWindowSize(display.window, &w, &h);

	if(display.width == w && display.height == h)
		return;

	// Update window size and viewport
	display.width = w;
	display.height = h;
	glViewport(0, 0, w, h);

	// Update projection matrix
	projection = glm::perspective(glm::radians(90.0f), (float)w / (float)h, 0.01f, 1000.0f);

	// Update FBO color attachment
	glActiveTexture(GL_TEXTURE0 + fbo.textureIndex);
	glBindTexture(GL_TEXTURE_2D, fbo.color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	// Attach it to the FBO (TODO: Is it necessary?)
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo.color, 0);

	// Update renderbuffer for depth and stencil attachments
	glBindRenderbuffer(GL_RENDERBUFFER, fbo.depth24stencil8);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attach it to the FBO (TODO: is it necessary?)
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo.depth24stencil8);
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

		char buffer[512];
		sprintf(buffer, "%s | FPS: %.2lf | Frame Time: %.3lf", display.title, fps, frameTime);
		glfwSetWindowTitle(display.window, buffer);

		display.numFrames = 0;
		display.lastTime = currentFrame;
	}
}

FBO_Data CreateFBO(Display& display, Scene& scene)
{
	// Create framebuffer object
	GLuint FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Generate a color buffer attachment for the FBO
	GLuint FBOTextureIndex = (int)scene.textures.size();
	GLuint textureColorBuffer;
	glGenTextures(1, &textureColorBuffer);
	glActiveTexture(GL_TEXTURE0 + FBOTextureIndex);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, display.width, display.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach it to the FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

	// Because the depth buffer and stencil buffer won't be sampled,
	// we can use a renderbuffer object instead of a texture
	GLuint RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, display.width, display.height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// Attach it to the FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer object is not complete!\nReverting to default framebuffer");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	return { FBO, textureColorBuffer, FBOTextureIndex, RBO };
}
