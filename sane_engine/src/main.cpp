#include <Display/display.h>
#include <Shader/shader.h>
#include <Mesh/mesh.h>
#include <cstdio>

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

	Shader defaultShader = LoadShaderFromFile("res/shaders/defaultvs.glsl", "res/shaders/defaultfs.glsl");
	glUseProgram(defaultShader.ID);

	Mesh square = GenerateMesh(vertices, sizeof(vertices) / sizeof(float), indices, sizeof(indices) / sizeof(float));

	while(!glfwWindowShouldClose(display.window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		DrawMesh(&square);

		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}