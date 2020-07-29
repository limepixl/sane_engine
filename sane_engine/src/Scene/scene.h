#pragma once
#include <vector>
#include <Mesh/mesh.h>
#include <Texture/texture.h>
#include <glm/vec3.hpp>

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
	glm::vec3 lightPos;
};

void DrawScene(Scene& scene, struct Shader& shader);