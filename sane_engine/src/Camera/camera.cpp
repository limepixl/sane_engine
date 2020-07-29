#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

glm::mat4 GetViewMatrix(Camera& camera)
{
    return glm::lookAt(camera.position, camera.position + camera.forward, camera.up);
}