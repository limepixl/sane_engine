#include "scene.h"
#include <Shader/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// NOTE: Currently the scene is being drawn
// one mesh at a time. For example, if there
// are 3 dragons, 2 chairs and 5 teapots in
// the scene, all entities that use a mesh
// will be drawn before the next mesh is bound.
//
// TODO: Find a more efficient way to do this
// Look into batching?
void DrawScene(Scene& scene, Shader& shader)
{
	int lastMeshIndex = -1;
	for(auto& e : scene.entities)
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, e.position);
		model = glm::rotate(model, glm::radians(e.rotation.x), { 1.0, 0.0, 0.0 });
		model = glm::rotate(model, glm::radians(e.rotation.y), { 0.0, 1.0, 0.0 });
		model = glm::rotate(model, glm::radians(e.rotation.z), { 0.0, 0.0, 1.0 });
		model = glm::scale(model, e.scale);
		glUniformMatrix4fv(shader.locations["model"], 1, GL_FALSE, &model[0][0]);

		glUniform1i(shader.locations["diffusetex"], e.diffuseIndex);
		glUniform1i(shader.locations["speculartex"], e.specularIndex);

		// NOTE: Uncomment this if moving lights are in the scene
		// for(size_t i = 0; i < scene.lights.size(); i++)
		//   glUniform3fv(shader.locations["lightPositions[0]"] + (GLint)i, 1, &scene.lights[i][0]);

		MeshIndexed& mesh = scene.meshes[e.meshIndex];
		if(lastMeshIndex != (int)e.meshIndex)
		{
			DrawMeshIndexed(mesh, false);
			lastMeshIndex = e.meshIndex;

			continue;
		}
		
		DrawMeshIndexed(mesh, true);
	}

	glBindVertexArray(0);
}

void DrawLights(Scene& scene, Shader& shader, Mesh& mesh)
{
	bool drawnOnce = false;
	for(auto& pos : scene.lights)
	{
		glm::mat4 model(1.0);
		model = glm::translate(model, pos);
		glUniformMatrix4fv(shader.locations["model"], 1, GL_FALSE, &model[0][0]);

		DrawMesh(mesh, drawnOnce);
		drawnOnce = true;
	}
}
