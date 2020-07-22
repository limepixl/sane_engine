#include <Display/display.h>
#include <Shader/shader.h>
#include <Mesh/mesh.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main()
{
	Display display = CreateDisplay(800, 600, "Sane Engine");

	float vertices[]
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0,
		-0.5f, 0.5f, 0.0f
	};

	unsigned int indices[]
	{
		0, 1, 2,
		2, 3, 0
	};

	Mesh square = GenerateMesh(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(float));
	
	Shader default3DShader = LoadShaderFromFile("res/shaders/default3Dvs.glsl", "res/shaders/default3Dfs.glsl");
	glUseProgram(default3DShader.ID);

	glm::mat4 model(1.0f);
	SetMat4(model, "model", &default3DShader);

	glm::mat4 view(1.0f);
	view = glm::translate(view, { 0.0f, 0.0f, -3.0f });
	SetMat4(view, "view", &default3DShader);

	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)display.width / (float)display.height, 0.1f, 1000.0f);
	SetMat4(projection, "projection", &default3DShader);

	while(!glfwWindowShouldClose(display.window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawMesh(&square);

		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}