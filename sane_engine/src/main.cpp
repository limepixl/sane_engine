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
	Texture cubemap = LoadCubemapFromFile(paths, 6, scene.textures.size() + 1);
	Mesh cubemapMesh = GenerateCube();
	Shader cubemapShader = LoadShaderFromFile("res/shaders/skybox/skyboxvs.glsl", "res/shaders/skybox/skyboxfs.glsl");

	// Create framebuffer object
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	// Generate a color buffer attachment for the FBO
	GLuint FBOTextureIndex = (int)scene.textures.size();
	GLuint textureColorBuffer;
	glGenTextures(1, &textureColorBuffer);
	glActiveTexture(GL_TEXTURE0 + FBOTextureIndex);
	glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, display.width, display.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE0);

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

	while(!glfwWindowShouldClose(display.window))
	{
		DeltaTimeCalc(display);
		ProcessInput(display, camera);

		// Draw scene to framebuffer object
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view = GetViewMatrix(camera);
		glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)display.width / (float)display.height, 0.01f, 1000.0f);
		glm::mat4 nonTranslatedView = glm::mat4(glm::mat3(view));

		// Firstly, draw the cubemap
		glDepthMask(GL_FALSE);
		glUseProgram(cubemapShader.ID);
		glUniformMatrix4fv(cubemapShader.locations["projection"], 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(cubemapShader.locations["view"], 1, GL_FALSE, &nonTranslatedView[0][0]);
		glUniform1i(cubemapShader.locations["cubemap"], cubemap.index);
		DrawMesh(cubemapMesh, false);
		glDepthMask(GL_TRUE);

		// Next, draw the whole scene
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

		// Switch to default framebuffer object and draw the 
		// other framebuffer object's color buffer onto this one
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		
		glUseProgram(screenQuadShader.ID);
		glUniform1i(screenQuadShader.locations["fbo"], FBOTextureIndex);

		DrawMesh(screenQuadMesh, false);

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