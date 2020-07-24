#include "scene.h"
#include <Shader/shader.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// TODO: have some sort of cache. 
// For example, if the same mesh is being drawn
// for the next entity and the current entity,
// the next entity shouldn't bind the mesh.
void DrawScene(Scene& scene, Shader& shader)
{
	for(auto& e : scene.entities)
	{
		glm::mat4 model(1.0f);
		model = glm::translate(model, e.position);
		model = glm::rotate(model, glm::radians(e.rotation.x), { 1.0, 0.0, 0.0 });
		model = glm::rotate(model, glm::radians(e.rotation.y), { 0.0, 1.0, 0.0 });
		model = glm::rotate(model, glm::radians(e.rotation.z), { 0.0, 0.0, 1.0 });
		model = glm::scale(model, e.scale);
		SetMat4(model, "model", shader);

		SetInt(e.textureIndex, "tex", shader);
		BindTexture(scene.textures[e.textureIndex]);
		DrawMesh(scene.meshes[e.meshIndex]);
	}
}
