#include <glm/gtc/matrix_transform.hpp>
#include <ResManagement/resource_management.h>
#include <Display/display.h>
#include <cstdio>

int main()
{
	Display display = CreateDisplay(1280, 720, "Sane Engine");

	Shader shader = LoadShaderFromFile("res/shaders/texture3Dvs.glsl", "res/shaders/texture3Dfs.glsl");
	glUseProgram(shader.ID);

	glm::mat4 view(1.0f);
	view = glm::translate(view, { 0.0f, 0.0f, -2.0f });
	glUniformMatrix4fv(shader.locations["view"], 1, GL_FALSE, &view[0][0]);

	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)display.width / (float)display.height, 0.1f, 1000.0f);
	glUniformMatrix4fv(shader.locations["projection"], 1, GL_FALSE, &projection[0][0]);

	Scene scene1 = LoadSceneFromFile("res/scenes/scene1.txt");

	while(!glfwWindowShouldClose(display.window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DrawScene(scene1, shader);

		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	// Clean up
	for(auto& m : scene1.meshes)
		Clean(m);
	for(auto& t : scene1.textures)
		Clean(t);

	glfwTerminate();
	return 0;
}