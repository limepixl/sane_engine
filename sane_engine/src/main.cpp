#include <glm/gtc/matrix_transform.hpp>
#include <ResManagement/resource_management.h>
#include <Display/display.h>
#include <Camera/camera.h>
#include <cstdio>

int main()
{
	Display display = CreateDisplay(1280, 720, "Sane Engine");

	Camera camera{ {0.0f, 0.0f, 2.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 0.0f, -90.0f };

	Shader lightShader = LoadShaderFromFile("res/shaders/light/lightvs.glsl", "res/shaders/light/lightfs.glsl");
	Mesh lightMesh = LoadMeshFromOBJ("res/models/cube.obj");

	Shader shader = LoadShaderFromFile("res/shaders/normals/normalsvs.glsl", "res/shaders/normals/normalsfs.glsl");

	Scene scene = LoadSceneFromFile("res/scenes/scene.txt");

	while(!glfwWindowShouldClose(display.window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		DeltaTimeCalc(display);
		ProcessInput(display, camera);

		glUseProgram(shader.ID);

		glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)display.width / (float)display.height, 0.01f, 1000.0f);
		glUniformMatrix4fv(shader.locations["projection"], 1, GL_FALSE, &projection[0][0]);

		// Update camera uniforms
		glm::mat4 view = GetViewMatrix(camera);
		glUniformMatrix4fv(shader.locations["view"], 1, GL_FALSE, &view[0][0]);
		glUniform3fv(shader.locations["cameraPos"], 1, &camera.position[0]);

		DrawScene(scene, shader);

		glUseProgram(lightShader.ID);
		glUniformMatrix4fv(lightShader.locations["projection"], 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(lightShader.locations["view"], 1, GL_FALSE, &view[0][0]);

		DrawLights(scene, lightShader, lightMesh);

		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	// Clean up
	for(auto& m : scene.meshes)
		Clean(m);
	for(auto& t : scene.textures)
		Clean(t);

	glfwTerminate();
	return 0;
}