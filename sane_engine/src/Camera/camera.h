#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct Camera
{
	glm::vec3 position;
	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;

	float pitch;
	float yaw;
};

glm::mat4 GetViewMatrix(Camera& camera);