#include <glm/gtc/matrix_transform.hpp>
#include <ResManagement/resource_management.h>
#include <Display/display.h>
#include <Camera/camera.h>
#include <GLFW/glfw3.h>
	
unsigned int Texture::GlobalTextureCount = 0;

int main()
{
	Display display = CreateDisplay(1280, 720, "Sane Engine");
	Camera camera{ {0.0f, 0.0f, 2.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 0.0f, -90.0f };

	// Shader and mesh used for drawing lights in the world
	Shader lightShader = LoadShaderFromFile("res/shaders/light/lightvs.glsl", "res/shaders/light/lightfs.glsl");
	Mesh lightMesh = LoadMeshFromOBJ("res/models/cube.obj");

	// Shader and scene used for drawing everything else in the scene
	Shader shader = LoadShaderFromFile("res/shaders/normals/normalsvs.glsl", "res/shaders/normals/normalsfs.glsl");
	Scene scene = LoadSceneFromFile("res/scenes/stressTest.txt");

	// Pass light positions to main shader for light calculations
	glUseProgram(shader.ID);
	for(size_t i = 0; i < scene.lights.size(); i++)
	{
		glUniform3fv(shader.locations["lightPositions[0]"] + (GLint)i, 1, &scene.lights[i][0]);
	}

	// Shader and mesh for the screen quad that is the canvas for the FBOs
	Shader screenQuadShader = LoadShaderFromFile("res/shaders/screenQuad/screenQuadvs.glsl", "res/shaders/screenQuad/screenQuadfs.glsl");
	Mesh screenQuadMesh = GenerateScreenQuad();

	std::string paths[] =
	{
		"res/skybox/sea/right.jpg",
		"res/skybox/sea/left.jpg",
		"res/skybox/sea/top.jpg",
		"res/skybox/sea/bottom.jpg",
		"res/skybox/sea/front.jpg",
		"res/skybox/sea/back.jpg"
	};
	Texture cubemap = LoadCubemapFromFile(paths, 6, Texture::GlobalTextureCount++);
	Mesh cubemapMesh = GenerateCube();
	Shader cubemapShader = LoadShaderFromFile("res/shaders/skybox/skyboxvs.glsl", "res/shaders/skybox/skyboxfs.glsl");
	
	FBO_Data FBO = CreateFBO(display, scene);
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)display.width / (float)display.height, 0.01f, 1000.0f);
	
	while(!glfwWindowShouldClose(display.window))
	{
		DeltaTimeCalc(display);
		ProcessInput(display, camera);
		CheckForResize(display, FBO, projection);

		// Draw scene to framebuffer object
		glBindFramebuffer(GL_FRAMEBUFFER, FBO.FBO);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = GetViewMatrix(camera);
		glm::mat4 nonTranslatedView = glm::mat4(glm::mat3(view));

		// Firstly, draw the whole scene
		glUseProgram(shader.ID);
		glUniformMatrix4fv(shader.locations["projection"], 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(shader.locations["view"], 1, GL_FALSE, &view[0][0]);
		glUniform3fv(shader.locations["cameraPos"], 1, &camera.position[0]);
		DrawScene(scene, shader);

		// Next, draw the lights as cubes
		glUseProgram(lightShader.ID);
		glUniformMatrix4fv(lightShader.locations["projection"], 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(lightShader.locations["view"], 1, GL_FALSE, &view[0][0]);
		DrawLights(scene, lightShader, lightMesh);

		// Lastly, draw the skybox wherever the background color is visible
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		glUseProgram(cubemapShader.ID);
		glUniformMatrix4fv(cubemapShader.locations["projection"], 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(cubemapShader.locations["view"], 1, GL_FALSE, &nonTranslatedView[0][0]);
		glUniform1i(cubemapShader.locations["cubemap"], cubemap.index);
		DrawMesh(cubemapMesh, false);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		// Switch to default framebuffer object and draw the 
		// other framebuffer object's color buffer onto this one
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		
		glUseProgram(screenQuadShader.ID);
		glUniform1i(screenQuadShader.locations["fbo"], FBO.textureIndex);

		DrawMesh(screenQuadMesh, false);

		glfwSwapBuffers(display.window);
		glfwPollEvents();
	}

	// Clean up
	for(auto& m : scene.meshes)
		CleanMeshIndexed(m);
	for(auto& t : scene.textures)
		CleanTexture(t);

	glfwTerminate();
	return 0;
}