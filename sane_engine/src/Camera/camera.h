#pragma once
#include <glm/glm.hpp>

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