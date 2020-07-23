#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include <ResManagement/resource_management.h>
#include <Display/display.h>
#include <Mesh/mesh.h>
#include <cstdio>

int main()
{
	Display display = CreateDisplay(1280, 720, "Sane Engine");

	Shader texture3D = LoadShaderFromFile("res/shaders/normalsvs.glsl", "res/shaders/normalsfs.glsl");
	glUseProgram(texture3D.ID);

	glm::mat4 view(1.0f);
	view = glm::translate(view, { 0.0f, 0.0f, -2.0f });
	SetMat4(view, "view", texture3D);

	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)display.width / (float)display.height, 0.1f, 1000.0f);
	SetMat4(projection, "projection", texture3D);

	Mesh dragon = LoadMeshFromOBJ("res/models/dragon.obj");
	Texture texture = LoadTextureFromFile("res/image/dragon_texture_color.png", 0);
	BindTexture(texture);
	SetInt(0, "tex", texture3D);

	while(!glfwWindowShouldClose(display.window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 model(1.0f);
		model = glm::rotate(model, glm::radians((float)glfwGetTime() * 50.0f), { 0.0, 1.0, 0.0 });
		SetMat4(model, "model", texture3D);

		DrawMesh(dragon);

		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}