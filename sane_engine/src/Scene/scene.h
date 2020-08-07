#pragma once
#include <vector>
#include <glm/vec3.hpp>
#include <Mesh/mesh.h>
#include <Texture/texture.h>

struct Entity
{
	unsigned int meshIndex, diffuseIndex, specularIndex;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

struct Scene
{
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;
	std::vector<Entity> entities;
	std::vector<glm::vec3> lights;
};

void DrawScene(Scene& scene, struct Shader& shader);
void DrawLights(Scene& scene, struct Shader& shader, struct Mesh& mesh);