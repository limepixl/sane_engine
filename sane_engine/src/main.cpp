#include <Display/display.h>
#include <Shader/shader.h>
#include <Mesh/mesh.h>
#include <Texture/texture.h>
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

	float texCoords[]
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	unsigned int indices[]
	{
		0, 1, 2,
		2, 3, 0
	};

	Mesh square = GenerateMesh(vertices, 12, texCoords, 8, indices, 6);
	
	Shader texture3D = LoadShaderFromFile("res/shaders/texture3Dvs.glsl", "res/shaders/texture3Dfs.glsl");
	glUseProgram(texture3D.ID);

	glm::mat4 model(1.0f);
	SetMat4(model, "model", &texture3D);

	glm::mat4 view(1.0f);
	view = glm::translate(view, { 0.0f, 0.0f, -2.0f });
	SetMat4(view, "view", &texture3D);

	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)display.width / (float)display.height, 0.1f, 1000.0f);
	SetMat4(projection, "projection", &texture3D);

	Texture texture = LoadTextureFromFile("res/image/dog.jpg", 0);
	SetInt(0, "tex", &texture3D);
	BindTexture(&texture);

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