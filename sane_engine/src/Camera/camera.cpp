#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

glm::mat4 GetViewMatrix(Camera& camera)
{
    return glm::lookAt(camera.position, camera.position + camera.forward, camera.up);
}

// TODO: Deep dive into this to understand the math better
void UpdateVectors(Camera& camera)
{
    if(camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.forward = glm::normalize(direction);
}
